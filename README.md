# DatabaseCopyingUtility
This is an instrument developed to make copying data from one database to another quick and easy

Now you can easily move data from one .db file to another without any default SQL clients
## How do i do this?
1) Click browse buttons in application and choose SQLite database files via dialog window 
2) Choose table 
3) Choose column you want to move
4) Press copy button 
5) ~~Relax :)~~ 

If you made a mistake choosing column just click clear button! Now you can ~~enjoy your life~~ continue your ~~boring~~ work c:
        

Screenshots provide will provide you with the general look of this application
![alt tag](Ashampoo_Snap_2018.09.19_21h41m43s_003_.png)

# Please notice
This app is still in development, copying/clearing will be unavailiable until next week, other features work just fine

# Built With
- [Qt](http://www.qt.io/) - user interface and internal logic
- [SQLite Maestro](https://www.sqlmaestro.com/products/sqlite/maestro/) - used to create a database for this project

- Qt documentation - [click this link](http://doc.qt.io/)

# Database 
Database gives user the ability to store information on cosmic vehicles, its systems, subsystems and commands/telemetry data.
  ## Database structure
Contains 5 tables: 
- KA (Kosmic Apparat)
    rows:
    - id - int (not displayed) - foreign key for Systems id_ka
    - ka - String
- System 
    - id - int (not displayed) - foreign key for SubSystems id_subsystem
    - id_ka - int
    - address - String
    - name - String
- SubSystem
    - id - int (not displayed) - foreign key for TMI/Command id_subsystem
    - id_subsystem - int
    - subaddress - String
    - count_data_word - String
    - name - String
- TMI
    - id - int (not displayed)
    - id_subsystem - int
    - number_parameter - int
    - count_bit - int
    - number_bit - int
    - number_data_word - int
    - name - String
    - description - String
- Command
    - id - int (not displayed)
    - id_subsystem - int
    - count_bit - int
    - number_bit - int
    - number_data_word - int
    - reaction_time - int
    - name - String
    - description - String
    
    Please notice that for convenience' sake program displays SubSystem's name in the field id_subsystem of QTableView
# Getting started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.
This application requires SQLite client(e.g. SQLite Maestro).

## Installing
Create new database with instructions above or just use '.db' file from this repository.
Download and run Qt Creator, download this project, then open '.pro' file, setup your project. 
In your project change path to '.db' file to match the directory you store the database in.
Now you're ready to go!

# Authors
- Dmitry Petukhov - Initial work/founder

# Acknowledgments
  - You can help me to develop this program or to make it better for your puproses 
  - Feel free to make it look the way you like
 
