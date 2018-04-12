# MedicationReminder

This is a medication reminder robot. I used Arduino Uno to build the robot. What it actually does is that it reads a certain .txt file,
where the medicine sotrage(number of medicines the user has), medicine taking time and amount of that particular medicine to take are all
stored in Json format.
Arduino reads the file and gives alarm at that particalar time to remind the user to take his/her medicine.

There's also a pc app thorugh which  the user can update or remove any medicine he wants. He can also change the time for taking medicine.
I used Java to build this app.

To reduce power consumption I initiated the arduino sleep_mode.
