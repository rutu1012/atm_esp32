# ESP32 based ATM system
* An ATM machine system has been implemented using ESP32 where the user makes their transactions with the help of a telegram chatbot and they get updated onto the google sheet linked with it.

## User Input (Telegram)
* For the user to login to the ATM machine, we take input to the esp32 by integrating it with our own telegram bot.
* ESP32 will be able to interact with the bot and thus will be able to take in our input and give appropriate response.
* To activate the bot, we type ’/start’ which displays the message for the user to login and the further functions it can do.
* We send our username and password to the telegram bot which gets stored in a variable for verification purpose.

## Password Checking (with Google Sheet)
* On receiving the input from user, we crosscheck it with the data present in the google sheet named ”login” already containing the login details. The details of username,password and opening balance is set from the sheet. The login data is set and the opening balance is set to Rs.15000.\\
Google sheet link: https://docs.google.com/spreadsheets/d/1NEXAgkDvvR9sJaMHwiI_EgIfJ7B_oycioqkUwkHnRLE/edit?usp=sharing

## Making Transactions
* We further give 2 options to make transactions of withdraw and deposit
* The No. of Rs100 notes to withdraw/deposit is also input from the telegram bot since typed input through the bot is much more user friendly than touchpins .
* The summary of the transaction containing the opening balance, current balance, amount withdrawn/deposited and the final remaining balance is displayed on telegram and the Serial Monitor. The Google sheet is also updated with these details in a tabular format. 
* The user also has the feature to check his current balance details which shows his/her remaining balance.

## Send Receipt to Mail (additional feature)
* When the user logs out from the system, a mail is sent the user’s email id containing the receipt of the last transactions done on the atm system.


Youtube video: https://youtu.be/DsIu739TdrU
