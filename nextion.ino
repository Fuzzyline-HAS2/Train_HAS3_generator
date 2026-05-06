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
        sendCommand("pleftDevice.pic=168");
        Serial.println("left Generator 5");
    }
    else if((int)my["left_generator"] == 4)
    {
        sendCommand("pleftDevice.pic=169");
        Serial.println("left Generator 4");
    }
    else if((int)my["left_generator"] == 3)
    {
        sendCommand("pleftDevice.pic=170");
        Serial.println("left Generator 3");
    }
    else if((int)my["left_generator"] == 2)
    {
        sendCommand("pleftDevice.pic=171");
        Serial.println("left Generator 2");
    }
    else if((int)my["left_generator"] == 1)
    {
        sendCommand("pleftDevice.pic=172");
        Serial.println("left Generator 1");
    }
}
void BatteryPackSend(){
    if((String)(const char*)my["battery_pack"] == "0")
    {
        sendCommand("tBattery0.en=0");
    }
    else if((String)(const char*)my["battery_pack"] == "1")
    {
        sendCommand("tBattery0.en=1");
    }
    else if((String)(const char*)my["battery_pack"] == "2")
    {
        sendCommand("tBattery1.en=1");
    }
    else if((String)(const char*)my["battery_pack"] == "3")
    {
        sendCommand("tBattery2.en=1");
    }
    else if((String)(const char*)my["battery_pack"] == "4")
    {
        sendCommand("tBattery3.en=1");
    }
    else if((String)(const char*)my["battery_pack"] == "5")
    {
        sendCommand("vBatteryPack.val=5");
    }
}
