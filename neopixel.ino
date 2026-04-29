void NeoLightColor(int neoIdx, int* rgb) {
  pixels[neoIdx].fill(pixels[neoIdx].Color(rgb[0], rgb[1], rgb[2]));
  pixels[neoIdx].show();
}

void NeopixelInit()
{
  for (int i = 0; i < NeopixelNum; ++i)
  {
    pixels[i].begin();
    pixels[i].setBrightness(ledBrightness);
  }
  for (int i = 0; i < NeopixelNum; ++i)
  {
    NeoLightColor(i, color[WHITE]);
  }
}

void UpdateBrightness()
{
  int serverBrightness = my["brightness"].as<int>();
  if (serverBrightness <= 0 || serverBrightness > 100) {
    ledBrightness = DEFAULT_BRIGHTNESS;
  } else {
    ledBrightness = map(serverBrightness, 1, 100, 1, 255);
  }
  for (int i = 0; i < NeopixelNum; ++i) {
    pixels[i].setBrightness(ledBrightness);
    pixels[i].show();
  }
}

void EncoderNeopixelOn(int neoNum){
  for(int i = 0; i < neoNum; i++)
    pixels[GAUGE].setPixelColor(i,pixels[GAUGE].Color(color[BLUE][0], color[BLUE][1], color[BLUE][2]));
  for(int i = neoNum; i < NumPixels[GAUGE]; i++)
    pixels[GAUGE].setPixelColor(i,pixels[GAUGE].Color(color[GREEN][0],color[GREEN][1],color[GREEN][2])); 
  pixels[GAUGE].show();
}
void NeoBlink(int neo, int neoColor, int cnt, int blinkTime){
  for(int i = 0; i < cnt; i++){                          //0.5*10=5초동안 점멸
    NeoLightColor(neo, color[BLACK]); //전체 off
    delay(blinkTime);
    NeoLightColor(neo, color[neoColor]); //전체 적색on
    delay(blinkTime);                   //전체 적색on
  }
}

void AllNeoOn(int neoColor){
  for (int i = 0; i < NeopixelNum; ++i)
    NeoLightColor(i, color[neoColor]);
}
