import sys
import hmac
import hashlib
import os

# Windows CMD에서 이모지 출력을 위한 설정
sys.stdout.reconfigure(encoding='utf-8')

SCRIPTS_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, SCRIPTS_DIR)

try:
    from secrets import HMAC_SECRET
except ImportError:
    print("❌ scripts/secrets.py 파일이 없습니다.")
    print("   scripts/secrets.py.example 을 복사해서 secrets.py를 만들고 HMAC_SECRET을 입력해주세요.")
    sys.exit(1)

def sign_firmware(bin_path):
    if not os.path.exists(bin_path):
        print(f"❌ 파일을 찾을 수 없습니다: {bin_path}")
        sys.exit(1)

    with open(bin_path, "rb") as f:
        firmware = f.read()

    sig = hmac.new(HMAC_SECRET.encode('utf-8'), firmware, hashlib.sha256).hexdigest()

    sig_path = os.path.splitext(bin_path)[0] + ".sig"
    # update.bin → update.sig (같은 폴더)
    base_dir = os.path.dirname(bin_path)
    sig_path = os.path.join(base_dir, "update.sig")

    with open(sig_path, "w", encoding="utf-8") as f:
        f.write(sig)

    print(f"✅ 서명 완료: update.sig")
    print(f"   HMAC-SHA256: {sig}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("사용법: python sign_firmware.py <firmware.bin>")
        sys.exit(1)
    sign_firmware(sys.argv[1])
