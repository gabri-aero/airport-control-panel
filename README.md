# Control Panel

## Simulation of the control panel of an airport getting data from a txt file.

### Description.

Two main screens are displayed: the arrivals one and the departure one.
CTRL key is used to change between one another.

In every screen there are different 'sub-screens' which displayed the following information:
  
  1. Departures:
      -Check-in info: check-in desk, estimated time left, open/closed, estimated open time.
      -Boarding info: boarding gate, estimated open time, estimated time left, open/closed, LAST CALL.
      -Take-off info: plane state: on land, taking-off, already took-off... + estimated times.
  2. Arrivals:
      -Luggage info: luggage belt, estimated times, open/closed
      -Landing info: landing gate and plane state: already in air, landing, on land... + estimated time.
      
Sub-screens are changed automathically every little time.
      
Planes information is read from two differents .txt files. which contain about 40 different planes info each.

Plus, this information can be modified with the alert menu. Press ALT to deploy it.
Then you can introduce the number of the plane whose information you want to change.
*NOTE!* If you want to change the first plane shown press '1', if you want to change the second plane in the list press '2' and so on.

Once you have chosen the plane, you will have to introduce the corresponding alert code. It is shown below:
ALERT CODE:
  0. Delete a previous alert.
  1. Delay. Then you will have to introduce the minutes of delay.
  2. Flight cancelled.
  3. Change gate (landing gate or boarding gate depending on the screen you are). However, airport gates are available for both landing and boarding).
  4. Change check-in gate (departures) or luggage belt (arrivals).



