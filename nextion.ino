void NextionInit()
{
   nexInit();
   nexHwSerial.begin(9600, SERIAL_8N1, NEXTIONHWSERIAL_RX_PIN, NEXTIONHWSERIAL_TX_PIN);
}

void DisplayCheck()
{
 while (nexHwSerial.available() > 0)
 {
   String nextion_string = nexHwSerial.readStringUntil(' ');
   NextionReceived(&nextion_string);
 }
}

void NextionReceived(String *nextion_string)
{
 if (*nextion_string == "test")
 {
   sendCommand("page pgItemTaken");
   sendCommand("wQuizSolved.en=1");
 }
}

void SendCmd(String command)
{
    String cmd = "";
    if (command.startsWith("page") && (String)(const char *)shift_machine["selected_language"] == "EN")
    {
        cmd = "page E"+ command.substring(5);
    }
    else
    {
        cmd = command;
    }
    sendCommand(cmd.c_str());
}
void LeftGenerator(){

    if((int)my["left_generator"] == 5)
    {
        sendCommand("pgLocked.vLeftGen.val=5");
        sendCommand("pleftDevice.pic=42");
        Serial.println("left Generator 5");
    }
    else if((int)my["left_generator"] == 4)
    {
        sendCommand("pgLocked.vLeftGen.val=4");
        sendCommand("pleftDevice.pic=41");
        Serial.println("left Generator 4");
    }
    else if((int)my["left_generator"] == 3)
    {
        sendCommand("pgLocked.vLeftGen.val=3");
        sendCommand("pleftDevice.pic=40");
        Serial.println("left Generator 3");
    }
    else if((int)my["left_generator"] == 2)
    {
        sendCommand("pgLocked.vLeftGen.val=2");
        sendCommand("pleftDevice.pic=39");
        Serial.println("left Generator 2");
    }
    else if((int)my["left_generator"] == 1)
    {
        sendCommand("pgLocked.vLeftGen.val=1");
        sendCommand("pleftDevice.pic=38");
        Serial.println("left Generator 1");
    }
}
void BatteryPackSend(){
    if((String)(const char*)my["battery_pack"] == "0")
    {
        sendCommand("pgUnlocked.vBatteryPack.val=0");
        sendCommand("pBattery.pic=22");
    }
    else if((String)(const char*)my["battery_pack"] == "1")
    {
        sendCommand("pgUnlocked.vBatteryPack.val=1");
        sendCommand("pBattery.pic=23");
    }
    else if((String)(const char*)my["battery_pack"] == "2")
    {
        sendCommand("pgUnlocked.vBatteryPack.val=2");
        sendCommand("pBattery.pic=24");
    }
    else if((String)(const char*)my["battery_pack"] == "3")
    {
        sendCommand("pgUnlocked.vBatteryPack.val=3");
        sendCommand("pBattery.pic=25");
    }
    else if((String)(const char*)my["battery_pack"] == "4")
    {
        sendCommand("pgUnlocked.vBatteryPack.val=4");
        sendCommand("pBattery.pic=26");
    }
    else if((String)(const char*)my["battery_pack"] == "5")
    {
        sendCommand("pgUnlocked.vBatteryPack.val=5");
        sendCommand("pBattery.pic=27");
    }
}
void PageSend(){
    if((String)(const char*)my["device_state"] == "activate"){
        sendCommand("pgLogin.device_state.val=0");
        if((int)my["battery_pack"] == (int)my["max_battery_pack"])
            sendCommand("pgLogin.device_state.val=3");
    }
    else if((String)(const char*)my["device_state"] == "battery_max")
        sendCommand("pgLogin.device_state.val=1");
    else if((String)(const char*)my["device_state"] == "starter_finish")
        sendCommand("pgLogin.device_state.val=2");
}