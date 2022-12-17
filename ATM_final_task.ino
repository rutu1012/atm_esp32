
#include "WiFi.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> 
#include <ArduinoJson.h>
#include <ESP_Mail_Client.h>

#define myBot "5627999128:AAHNZLPYtoGQuilwLX1qVWR1WcI3dTFZIUQ"
#define CHAT_ID "1379866460"

const char* ssid = "Rutu_wifi";
const char* password = "Rutu1234";

String GSCRIPT_ID_read = "AKfycbzJHWapgwXIg87vQ62-AtHZlXpIernpWjkNqQPB5ciUZLZBLgMASLTRrOgmb1I02dpL";
String GSCRIPT_ID_post ="AKfycbw6Uv3I2hGRDFRja9b8gEhgqbfFpt4pCOiTuaYLXBzOAQnAjqkYTSlWnF6tGhLf58bb";

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

#define ESP_MAIL "rutud1012@gmail.com"
#define ESP_MPASS "ozjrqegdnalnqdhk"
#define USER_MAIL "rutushrirame@gmail.com"

SMTPSession smtp;

String receipt[5];
int rc=0;

String receivedText="";
WiFiClientSecure client;
UniversalTelegramBot bot(myBot, client);

int botDelay = 1000;
unsigned long lastTimeBotRan;
String passw="";
String user="";
int msgFlag=0;
int uF=0;
int pF=0;
int nF=0;
int dF=0;
int tc=0;
int flag = 0;
int notes;
int amt;
int op_bal;
String ownID="";
String strsplit[5];
String dataFromSheet="";
String url = "https://script.google.com/macros/s/" + GSCRIPT_ID_read + "/exec?read";

int checkData() {
    HTTPClient http;
    Serial.println("Fetching data from sheet");
    http.begin(url.c_str()); //Specify the URL and certificate
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    String atm_pass;
    if (httpCode > 0) { //Check for the returning code
      dataFromSheet = http.getString();
      splitData(dataFromSheet);
      Serial.println(httpCode);
      Serial.println(dataFromSheet);
      String user_sheet = strsplit[0];
      String atm_pass = strsplit[1];
      op_bal = strsplit[2].toInt();
      amt= op_bal;
      Serial.println("\nSheet details");
      Serial.println(user_sheet);
      Serial.println(atm_pass);
      Serial.println(op_bal);
      
      
      Serial.println(user_sheet);
      Serial.println(user);
      if(user_sheet == user){
        Serial.println("User name is matched");
        Serial.println(atm_pass);
        Serial.println(passw);
        if(atm_pass==passw){
        Serial.println("Password has been verified");
        bot.sendMessage(ownID,"Your Password is verified. You can begin with transaction. \nPlease type /withdraw to debit and /deposit to credit", "");
        flag=1;
        }
        else{
        Serial.println("Password does not match. Unauthorized user");
        bot.sendMessage(ownID, "Password does not match. Unauthorized user", "");
        flag=0;
      }
        }
      else{
          bot.sendMessage(ownID, "Username does not match. Unauthorized user", "");
          flag=0;
          }
      
      return flag;
    }

    else {
      Serial.println("Error on HTTP request");
    }
    http.end();
    delay(1000);
 }

void splitData(String str){
  int sc = 0;
  while (str.length() > 0)
  {
    int index = str.indexOf('=');
    if (index == -1) 
    {
      strsplit[sc++] = str;
      break;
    }
    else
    {
      strsplit[sc++] = str.substring(0, index);
      str = str.substring(index+1);
    }
  }
}  
// to deal with new messages
void handleNewMsg(int numNewMsg) {
  Serial.println("New ");
  for (int i=0; i<numNewMsg; i++) {
    ownID = String(bot.messages[i].chat_id);
    if (ownID != CHAT_ID){
      bot.sendMessage(ownID, "Unauthorized user", "");
      continue;
    }
    
    receivedText = bot.messages[i].text;
    Serial.println(receivedText); // Print the received message
    String sm="";
    String from_name = bot.messages[i].from_name;

    if(uF==1){
      user = receivedText;
      Serial.print("User Name: ");
      Serial.println(user);
      uF=0;
      }
    else if(pF==1){
        passw = receivedText;
        Serial.print("Password : ");
        Serial.println(passw);
        pF=0;
        msgFlag=1;
      }
    if(nF==1){
      notes = receivedText.toInt();
      Serial.print("Notes withdrawn : ");
      Serial.println(notes);
      transaction("withdraw",notes);
      nF=0;
      }
    if(dF==1){
      notes = receivedText.toInt();
      Serial.print("Notes depositrd : ");
      Serial.println(notes);
      transaction("deposit",notes);
      dF=0;
      }
    if (receivedText == "/start") {
      sm += "Welecome to ATM of xyz bank " + from_name + ".\n";
      sm += "Please type \n";
      sm += "/user_name to input username \n";
      sm += "/password to input password\n";
      sm += "/balance to check account balance\n";
      sm += "/withdraw to withdraw money\n";
      sm += "/deposit to deposit money\n";
      sm += "/logout to logout from ATM";
      bot.sendMessage(ownID, sm, "");
      user="";
      passw="";
      notes=0;
    }
    if (receivedText == "/user_name") {
      bot.sendMessage(ownID, "Please enter your username", "");
      uF=1;
      delay(1000);
    }
    
    if (receivedText == "/password") {
        bot.sendMessage(ownID, "Please enter your password", "");
        delay(1000);
        pF=1;
       }
    if (receivedText == "/withdraw"){
      if(flag){
        bot.sendMessage(ownID, "Please enter no. of 100Rs notes to withdraw", "");
        delay(1000);
        nF=1;
        }
      else{
        bot.sendMessage(ownID, "Unauthorized user, cannot withdraw", "");
        nF=0;
        }
      }
      if (receivedText == "/deposit"){
      if(flag){
        bot.sendMessage(ownID, "Please enter no. of 100Rs notes to deposit", "");
        delay(1000);
        dF=1;
        }
      else{
        bot.sendMessage(ownID, "Unauthorized user, cannot deposit", "");
        dF=0;
        }
      }
     if (receivedText == "/balance"){
      if(flag){
        bot.sendMessage(ownID, "Hello "+user+" Your current account balance is: Rs."+String(amt),"");
        delay(1000);
        }
      else{
        bot.sendMessage(ownID, "Unauthorized user, cannot check balance", "");
        }
      }

    if (receivedText == "/logout"){
      bot.sendMessage(ownID, "The transaction is complete, collect the cash and card.\nThank you for using our ATM .", "");     
      amt = op_bal;
      flag=0;
      sendMail(); //send the receipt to user's mail id
      inputFromBot();//restart for different user
      }
      delay(1000);
    }
    
    
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  // connect to WiFi
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  Serial.flush();
  
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi is connected");
}
//to get status of sent mail
void smtp_status(SMTP_Status status){
  Serial.println(status.info());
  if (status.success()){
    // printing status
    SMTP_Result res = smtp.sendingResult.getItem(0);
    ESP_MAIL_PRINTF("Status: %s\n", res.completed ? "success" : "failed");
    ESP_MAIL_PRINTF("Recipient: %s\n", res.recipients.c_str());
    ESP_MAIL_PRINTF("Subject: %s\n", res.subject.c_str());
  }
}
void sendMail(){

  smtp.debug(1);
  smtp.callback(smtp_status);
  ESP_Mail_Session session;

  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = ESP_MAIL;
  session.login.password = ESP_MPASS;
  session.login.user_domain = "";

  SMTP_Message message;
  message.sender.name = "ESP32_ATM";
  message.sender.email = ESP_MAIL;
  message.subject = "ATM receipt";
  message.addRecipient("Rutu", USER_MAIL);

  String mailMsg = "Dear Account holder user:"+user+"\nFollowing is the receipt of previous transactions done on ESP-based ATM system\n." ;  
  for(int i=0;i<=rc;i++){
    mailMsg+= receipt[i]+"\n";
    }
  //send mail
  message.text.content = mailMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  if (!smtp.connect(&session))
    return;

  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
  }


void transaction(String task, int notes){
  int sum = notes*100;
  int wsum=0;
  int dsum=0;
  int prev_bal=amt;
  if (task=="withdraw"){
    wsum = notes*100;
    amt-= wsum;
    tc+=1;
    }
  if (task=="deposit"){
    dsum = notes*100;
    amt+= dsum;
    tc+=1; 
    }
  String msg = "Opening balance: "+String(op_bal)+"\nCurrent Balance: "+String(prev_bal)+"\nAmount "+task+": "+String(sum)+"\nRemaining Balance: Rs."+String(amt);
  bot.sendMessage(ownID, msg, "");
  Serial.println(msg);

  receipt[rc]="Opening balance: "+String(op_bal)+"  Current Balance: "+String(prev_bal)+" Amount "+task+": "+String(sum)+"  Remaining Balance: Rs."+String(amt);;
  rc+=1;
  String url_post = "https://script.google.com/macros/s/"+GSCRIPT_ID_post+
    "/exec?"+"count=" + String(tc) + "&currentBalance=" + String(amt)+"&amt_withdrawn="+String(wsum)+"&amt_deposited="+String(dsum)+"&finalBalance="+String(amt);
    Serial.print("POST data to spreadsheet:");
    Serial.println(url_post);
    HTTPClient http;
    http.begin(url_post.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS) ;
    int httpCode = http.GET() ;
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    http.end() ;
  }


void inputFromBot(){
  while(1){
    if (millis() > lastTimeBotRan + botDelay)  {
      Serial.print("Bot: ");
      Serial.println(bot.last_message_received);
      int numNewMsg = bot.getUpdates(bot.last_message_received + 1);
      while(numNewMsg) {
        Serial.println("got response");
        handleNewMsg(numNewMsg);
        numNewMsg = bot.getUpdates(bot.last_message_received + 1);
      }
      lastTimeBotRan = millis();
    }
    if(msgFlag){
      Serial.println("Login Input from bot is taken");
      Serial.print("Username: ");
      Serial.println(user);
      Serial.print("Password: ");
      Serial.println(passw);
      flag = checkData();
      msgFlag=0;  
      }
 }
  return;
  }
void loop() {
  inputFromBot();
  }
  
