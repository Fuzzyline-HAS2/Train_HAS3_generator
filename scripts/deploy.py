import os
import re
import sys
import shutil
import subprocess
import glob

# Windows CMD에서 이모지 출력을 위한 설정
sys.stdout.reconfigure(encoding='utf-8')

# ================= 설정 =================
SKETCH_FILE = "generator.ino"   # ← 기기마다 이 줄만 수정

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SCRIPTS_DIR = os.path.dirname(os.path.abspath(__file__))
SKETCH_PATH = os.path.join(BASE_DIR, SKETCH_FILE)
BUILD_DIR = os.path.join(BASE_DIR, "build")
ARDUINO_OUTPUT = os.path.join(BASE_DIR, "ArduinoOutput")
OUTPUT_FILENAME = "update.bin"
# =======================================

def get_firmware_ver():
    with open(SKETCH_PATH, "r", encoding="utf-8") as f:
        content = f.read()
    match = re.search(r'#define\s+FIRMWARE_VER\s+(\d+)', content)
    if match:
        return int(match.group(1))
    return None

def find_newest_bin():
    search_patterns = [
        os.path.join(BUILD_DIR, "**", "*.bin"),
        os.path.join(ARDUINO_OUTPUT, "**", "*.bin"),
        os.path.join(BASE_DIR, "*.bin"),
        os.path.join(BASE_DIR, "**", "*.bin"),
    ]
    candidates = []
    for pattern in search_patterns:
        candidates.extend(glob.glob(pattern, recursive=True))

    # 배포용 파일 및 불필요한 파일 제외
    candidates = [f for f in candidates if not f.endswith(OUTPUT_FILENAME)]
    candidates = [f for f in candidates if "merged" not in f]
    candidates = [f for f in candidates if "bootloader" not in f]
    candidates = [f for f in candidates if "partitions" not in f]
    candidates = [f for f in candidates if "boot_app" not in f]

    if not candidates:
        return None
    return max(candidates, key=os.path.getmtime)

def sign_firmware(bin_path):
    sign_script = os.path.join(SCRIPTS_DIR, "sign_firmware.py")
    result = subprocess.run(
        [sys.executable, sign_script, bin_path],
        capture_output=True, text=True, encoding='utf-8', cwd=SCRIPTS_DIR
    )
    if result.returncode != 0:
        print(f"❌ 서명 실패:\n{result.stderr}")
        sys.exit(1)
    print(result.stdout.strip())

def git_push(version):
    print("\n☁️  GitHub에 업로드 중...")
    try:
        version_file = os.path.join(BASE_DIR, "version.txt")
        with open(version_file, "w", encoding="utf-8") as f:
            f.write(str(version))
        print(f"📝 version.txt = {version}")

        # 특정 파일만 git add (secrets 등 민감 파일 제외)
        files_to_add = ["update.bin", "update.sig", "version.txt"]
        subprocess.run(["git", "add"] + files_to_add, cwd=BASE_DIR, check=True)
        subprocess.run(["git", "commit", "-m", f"Firmware Update v{version}"], cwd=BASE_DIR, check=True)
        subprocess.run(["git", "push"], cwd=BASE_DIR, check=True)
        print("✅ 업로드 완료!")
    except subprocess.CalledProcessError as e:
        print(f"❌ Git 오류 발생: {e}")
        print("Git이 설치되어 있고 저장소가 연결되어 있는지 확인해주세요.")

def main():
    print("🚀 SecureOTA 배포를 시작합니다...")

    # 1. FIRMWARE_VER 읽기
    ver = get_firmware_ver()
    if ver is None:
        print(f"❌ 오류: {SKETCH_FILE}에서 FIRMWARE_VER를 찾을 수 없습니다.")
        print("   generator.ino 상단에 #define FIRMWARE_VER <숫자> 가 있는지 확인하세요.")
        return
    print(f"📌 FIRMWARE_VER: {ver}")

    # 2. 컴파일 대기
    print("\n⏳ [행동 필요] Arduino IDE에서 'Sketch > Export Compiled Binary'를 실행해주세요.")
    print("   컴파일이 완료되면 엔터(Enter) 키를 눌러주세요...")
    input()

    # 3. .bin 파일 찾기
    print("🔎 빌드된 .bin 파일을 찾는 중...")
    bin_file = find_newest_bin()
    if not bin_file:
        print("❌ .bin 파일을 찾을 수 없습니다.")
        print("   Arduino IDE에서 'Sketch > Export Compiled Binary'를 실행했는지 확인하세요.")
        return
    print(f"   찾음: {bin_file}")

    # 4. update.bin으로 복사
    output_path = os.path.join(BASE_DIR, OUTPUT_FILENAME)
    try:
        shutil.copy2(bin_file, output_path)
        print(f"📦 update.bin 생성 완료")
    except Exception as e:
        print(f"❌ 파일 복사 실패: {e}")
        return

    # 5. HMAC-SHA256 서명
    print("🔏 HMAC-SHA256 서명 중...")
    sign_firmware(output_path)

    # 6. Git push (update.bin, update.sig, version.txt만)
    git_push(ver)

    print(f"\n🎉 배포 완료! v{ver} 이(가) GitHub에 업로드되었습니다.")
    print("   서버에서 device_state = github 를 전송하면 기기가 업데이트됩니다.")

if __name__ == "__main__":
    main()
