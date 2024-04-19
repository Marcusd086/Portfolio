/* 
   Name: Marcus D
   CSC-2212-02 
   School of Education Database Project 
*/

CREATE TABLE edTPA_Options
(
  Opt_ID INT NOT NULL,
  Opt_Req_Score INT NOT NULL,
  PRIMARY KEY (Opt_ID)
);

CREATE TABLE Major
(
  Major_ID INT NOT NULL,
  Major_Name CHAR(25) NOT NULL,
  PRIMARY KEY (Major_ID)
);

CREATE TABLE Tests
(
  Test_ID INT NOT NULL,
  Test_Name CHAR(8) NOT NULL,
  PRIMARY KEY (Test_ID)
);
/*
	CREATE TABLE Flagged_for_Testing
	(
  	Testing_Flag INT NOT NULL,
  	Flag_Meaning CHAR(100) NOT NULL,
  	PRIMARY KEY (Testing_Flag)
	);

	CREATE TABLE J_Year_Flagged
	(
  	Flagged_ID INT NOT NULL,
  	Flag_Reason CHAR(15),
  	PRIMARY KEY (Flagged_ID)
	);
*/
CREATE TABLE License_Tests
(
  LTest_ID INT NOT NULL,
  Test_Name CHAR(20) NOT NULL,
  Qualifying_Score INT NOT NULL,
  PRIMARY KEY (LTest_ID)
);

CREATE TABLE Advisor
(
  Advisor_ID INT NOT NULL,
  Advisor_FName CHAR(20) NOT NULL,
  Advisor_LName CHAR(20) NOT NULL,
  Advisor_Email CHAR(20) NOT NULL,
  Major_ID INT NOT NULL,
  PRIMARY KEY (Advisor_ID),
  FOREIGN KEY (Major_ID) REFERENCES Major(Major_ID)
);

CREATE TABLE Student
(
  Student_ID INT NOT NULL,
  Student_FName CHAR(20) NOT NULL,
  Student_LName CHAR(20) NOT NULL,
  Application_GPA FLOAT NOT NULL,
  Student_Email CHAR(20) NOT NULL,
  Graduation_Year INT NOT NULL,
  Status CHAR(10) NOT NULL,
  Applied CHAR(1) NOT NULL,
  Sophomore_Disposition CHAR(1) NOT NULL,
  Junior_Disposition INT NOT NULL,
  REC CHAR(1) NOT NULL,
  BG_CK CHAR(1) NOT NULL,
  ST_BG_CK CHAR(1) NOT NULL,
  C_Minus_or_Lower CHAR(1) NOT NULL,
  Conduct CHAR(1) NOT NULL,
  Advisor_ID INT NOT NULL,
  Major_ID INT NOT NULL,
  PRIMARY KEY (Student_ID),
  FOREIGN KEY (Advisor_ID) REFERENCES Advisor(Advisor_ID),
  FOREIGN KEY (Major_ID) REFERENCES Major(Major_ID)
);

CREATE TABLE edTPA_Scores
(
  Student_ID INT NOT NULL,
  Opt_ID INT NOT NULL,
  Score INT NOT NULL,
  PRIMARY KEY (Student_ID),
  FOREIGN KEY (Student_ID) REFERENCES Student(Student_ID),
  FOREIGN KEY (Opt_ID) REFERENCES edTPA_Options(Opt_ID)
);

CREATE TABLE Testing
(
  Student_ID INT NOT NULL,
  Test_ID INT NOT NULL,
  FOREIGN KEY (Test_ID) REFERENCES Tests(Test_ID),
  FOREIGN KEY (Student_ID) REFERENCES Student(Student_ID)
);

CREATE TABLE Courses_Completed
(
  Student_ID INT NOT NULL,
  EDU_1200 CHAR(1) NOT NULL,
  EDU_2100 CHAR(1) NOT NULL,
  EDU_2000 CHAR(1) NOT NULL,
  PRIMARY KEY (Student_ID),
  FOREIGN KEY (Student_ID) REFERENCES Student(Student_ID)
);

CREATE TABLE Flags
(
  Student_ID INT NOT NULL,
  Flag_Appt CHAR(15),
  Flag_Testing CHAR(100) NOT NULL,
  PRIMARY KEY (Student_ID),
  FOREIGN KEY (Student_ID) REFERENCES Student(Student_ID)
/*  FOREIGN KEY (Testing_Flag) REFERENCES Flagged_for_Testing(Testing_Flag),
  FOREIGN KEY (Flagged_ID) REFERENCES J_Year_Flagged(Flagged_ID) */
);

CREATE TABLE J_Year_GPA
(
  Student_ID INT NOT NULL,
  Beg_Fall_GPA FLOAT NOT NULL,
  End_Fall_GPA FLOAT NOT NULL,
  End_Spring_GPA FLOAT NOT NULL,
  PRIMARY KEY (Student_ID),
  FOREIGN KEY (Student_ID) REFERENCES Student(Student_ID)
);

CREATE TABLE Testing_for_License
(
  Student_ID INT NOT NULL,
  LTest_ID INT NOT NULL,
  Score INT NOT NULL,
  FOREIGN KEY (LTest_ID) REFERENCES License_Tests(LTest_ID),
  FOREIGN KEY (Student_ID) REFERENCES Student(Student_ID)
);

/* edTPA_Options Data */
INSERT INTO edTPA_Options (Opt_ID,Opt_Req_Score) VALUES (1,75);
INSERT INTO edTPA_Options (Opt_ID,Opt_Req_Score) VALUES (2,90);

/* Major Data */
INSERT INTO Major (Major_ID,Major_Name) VALUES (1,'Elementary Ed');
INSERT INTO Major (Major_ID,Major_Name) VALUES (2,'Special Ed AD');
INSERT INTO Major (Major_ID,Major_Name) VALUES (3,'Special Ed GC');			
INSERT INTO Major (Major_ID,Major_Name) VALUES (4,'Health and PE');
INSERT INTO Major (Major_ID,Major_Name) VALUES (5,'Middle Grades');			
INSERT INTO Major (Major_ID,Major_Name) VALUES (6,'Secondary Social Studies');
INSERT INTO Major (Major_ID,Major_Name) VALUES (7,'Secondary Biology');
INSERT INTO Major (Major_ID,Major_Name) VALUES (8,'Secondary English');
INSERT INTO Major (Major_ID,Major_Name) VALUES (9,'Secondary Math');

/* Tests Data */
INSERT INTO Tests (Test_ID,Test_Name) VALUES (1,'ACT');
INSERT INTO Tests (Test_ID,Test_Name) VALUES (2,'SAT');
INSERT INTO Tests (Test_ID,Test_Name) VALUES (3,'PRAXIS');

/*	   Flagged for Testing Data 
	INSERT INTO Flagged_for_Testing (Testing_Flag,Flag_Meaning) VALUES (1,'They have satisfied summer testing requirements and provide documentation of passing scores in the required tests');
	INSERT INTO Flagged_for_Testing (Testing_Flag,Flag_Meaning) VALUES (2,'Provide documentation that they have registered for the two required tests');
	INSERT INTO Flagged_for_Testing (Testing_Flag,Flag_Meaning) VALUES (3,'Have not yet passed all required tests but provide documentation that they have re-registered');
	INSERT INTO Flagged_for_Testing (Testing_Flag,Flag_Meaning) VALUES (4,'Awaiting scores from summer tests');

	   J_Year_Flagged Data 
	INSERT INTO J_Year_Flagged (Flagged_ID,Flag_Reason) VALUES (1,'NULL');
	INSERT INTO J_Year_Flagged (Flagged_ID,Flag_Reason) VALUES (2,'IP-Needs appt');
	INSERT INTO J_Year_Flagged (Flagged_ID,Flag_Reason) VALUES (3,'GPA-no appt');
*/
/* License_Tests Data */
INSERT INTO License_Tests (LTest_ID,Test_Name,Qualifying_Score) VALUES (1,'Praxis CKT Math',150);
INSERT INTO License_Tests (LTest_ID,Test_Name,Qualifying_Score) VALUES (2,'Pearson Math',227);
INSERT INTO License_Tests (LTest_ID,Test_Name,Qualifying_Score) VALUES (3,'Pearson Reading',229);

/* Advisor Data */
INSERT INTO Advisor (Advisor_ID,Advisor_FName,Advisor_LName,Advisor_Email,Major_ID) VALUES (1,'James','Albert','JAlbert@hpu.edu',1);
INSERT INTO Advisor (Advisor_ID,Advisor_FName,Advisor_LName,Advisor_Email,Major_ID) VALUES (2,'Robert','Cavendish','RCavendish@hpu.edu',1);
INSERT INTO Advisor (Advisor_ID,Advisor_FName,Advisor_LName,Advisor_Email,Major_ID) VALUES (3,'Sophie','Tarara','STarara@hpu.edu',1);
INSERT INTO Advisor (Advisor_ID,Advisor_FName,Advisor_LName,Advisor_Email,Major_ID) VALUES (4,'Jeremy','Vess','JVess@hpu.edu',1);
INSERT INTO Advisor (Advisor_ID,Advisor_FName,Advisor_LName,Advisor_Email,Major_ID) VALUES (5,'Micheal','T. Owens','MTOwens@hpu.edu',2);
INSERT INTO Advisor (Advisor_ID,Advisor_FName,Advisor_LName,Advisor_Email,Major_ID) VALUES (6,'Salina','Owens','SOwens@hpu.edu',2);
INSERT INTO Advisor (Advisor_ID,Advisor_FName,Advisor_LName,Advisor_Email,Major_ID) VALUES (7,'Michelle','Lambert','MLambert@hpu.edu',3);
INSERT INTO Advisor (Advisor_ID,Advisor_FName,Advisor_LName,Advisor_Email,Major_ID) VALUES (8,'Andy','Leak','ALeak@hpu.edu',3);
INSERT INTO Advisor (Advisor_ID,Advisor_FName,Advisor_LName,Advisor_Email,Major_ID) VALUES (9,'Chris','Johnson','CJohnson@hpu.edu',4);
INSERT INTO Advisor (Advisor_ID,Advisor_FName,Advisor_LName,Advisor_Email,Major_ID) VALUES (10,'Sarah','Albritton','SAlbritton@hpu.edu',1);
INSERT INTO Advisor (Advisor_ID,Advisor_FName,Advisor_LName,Advisor_Email,Major_ID) VALUES (11,'Johnny','Disseler','JDisseler@hpu.edu',2);
INSERT INTO Advisor (Advisor_ID,Advisor_FName,Advisor_LName,Advisor_Email,Major_ID) VALUES (12,'Sally','Summey','SSummey@hpu.edu',4);
INSERT INTO Advisor (Advisor_ID,Advisor_FName,Advisor_LName,Advisor_Email,Major_ID) VALUES (13,'Tim','O Hara','TOhara@hpu.edu',3);

/* Student Data */
INSERT INTO `Student` (`Student_ID`,`Student_FName`,`Student_LName`,`Application_GPA`,`Student_Email`,`Graduation_Year`,`Status`,`Applied`,`Sophomore_Disposition`,`Junior_Disposition`,`REC`,`BG_CK`,`ST_BG_CK`,`C_Minus_or_Lower`,`Conduct`,Advisor_ID,Major_ID) VALUES (1,"Kameko","Ebony",1,"Nam.consequat.dolor@semperegestasurna.edu",2024,"Admit","P","A",67,"P","P","P","P","P",1,1),(2,"Macaulay","Edan",1,"Phasellus.dapibus.quam@placerataugue.com",2018,"Admit","P","A",58,"P","P","P","P","P",2,1),(3,"Mannix","Ocean",3,"a.enim@felisNullatempor.edu",2021,"Admit","P","A",37,"P","P","P","P","P",2,1),(4,"Drew","Whitney",4,"Maecenas.iaculis@ametdapibus.co.uk",2017,"Admit","P","A",35,"P","P","P","P","P",3,1),(5,"Demetria","Reagan",4,"montes@mi.edu",2020,"Admit","P","A",70,"P","P","P","P","P",2,4),(6,"September","Lisandra",2,"risus.quis.diam@diam.org",2020,"Admit","P","A",56,"P","P","P","P","P",4,1),(7,"Linus","Reece",1,"arcu.Sed.et@disparturientmontes.net",2024,"Admit","P","A",31,"P","P","P","P","P",5,3),(8,"Akeem","Aidan",2,"et@tortor.com",2024,"Admit","P","A",39,"P","P","P","P","P",5,3),(9,"Carolyn","Donovan",4,"tincidunt@velitdui.co.uk",2024,"Admit","P","A",60,"P","P","P","P","P",2,3),(10,"Gavin","Mira",4,"Etiam.vestibulum@ut.ca",2018,"Admit","P","A",44,"P","P","P","P","P",6,1);
INSERT INTO `Student` (`Student_ID`,`Student_FName`,`Student_LName`,`Application_GPA`,`Student_Email`,`Graduation_Year`,`Status`,`Applied`,`Sophomore_Disposition`,`Junior_Disposition`,`REC`,`BG_CK`,`ST_BG_CK`,`C_Minus_or_Lower`,`Conduct`,Advisor_ID,Major_ID) VALUES (11,"Hayden","TaShya",3,"Praesent.interdum@augueporttitor.net",2016,"Prov","P","A",34,"P","P","P","P","P",6,2),(12,"Tanek","Declan",4,"commodo.ipsum.Suspendisse@cursuset.co.uk",2019,"Prov","P","A",59,"P","P","P","P","P",7,2),(13,"April","Daquan",2,"volutpat.ornare.facilisis@justo.edu",2023,"Prov","P","A",26,"P","P","P","P","P",8,1),(14,"Erasmus","Brock",3,"mi@lobortisquam.edu",2020,"Prov","P","A",37,"P","P","P","P","P",4,1),(15,"Gail","Kiayada",1,"torquent.per.conubia@idenimCurabitur.ca",2025,"Prov","P","A",49,"P","P","P","P","P",9,2),(16,"Branden","Elliott",4,"quis@tellusSuspendissesed.ca",2023,"Prov","P","A",44,"P","P","P","P","P",9,1),(17,"Cullen","Travis",1,"convallis.in@quis.edu",2016,"Prov","P","A",47,"P","P","P","P","P",6,1),(18,"Brenden","Rooney",3,"amet@nequeNullam.net",2025,"Prov","P","A",27,"P","P","P","P","P",9,2),(19,"Clinton","Maia",2,"fames.ac.turpis@nonarcuVivamus.ca",2024,"Prov","P","A",60,"P","P","P","P","P",10,1),(20,"Shad","Leonard",1,"mollis@tortor.co.uk",2017,"Prov","P","A",36,"P","P","P","P","P",11,1);

/* edTPA_Scores Data */
INSERT INTO edTPA_Scores (Student_ID,Opt_ID,Score) VALUES (1,1,50),(2,1,56),(3,2,89),(4,1,70),(5,2,35),(6,1,75),(7,1,75),(8,2,90),(9,1,40),(10,2,76);
INSERT INTO edTPA_Scores (Student_ID,Opt_ID,Score) VALUES (11,1,60),(12,2,67),(13,2,78),(14,2,80),(15,1,46),(16,2,75),(17,2,56),(18,2,78),(19,2,48),(20,1,75);

/* Testing Data */
Insert INTO Testing (Student_ID,Test_ID) VALUES (1,2),(1,3),(2,2),(2,3),(3,1),(4,2),(5,3),(6,1),(7,3),(8,2),(9,1),(9,3),(10,3);
Insert INTO Testing (Student_ID,Test_ID) VALUES (11,3),(12,3),(13,1),(14,3),(15,1),(16,1),(17,1),(17,3),(18,3),(19,2),(20,2);

/* Courses_Completed Data */
INSERT INTO `Courses_Completed` (Student_ID,`EDU_1200`,`EDU_2100`,`EDU_2000`) VALUES (1,"Y","Y","Y"),(2,"Y","Y","Y"),(3,"Y","Y","Y"),(4,"Y","Y","Y"),(5,"Y","N","Y"),(6,"N","Y","Y"),(7,"Y","Y","N"),(8,"Y","N","Y"),(9,"Y","Y","N"),(10,"Y","Y","Y");
INSERT INTO `Courses_Completed` (Student_ID,`EDU_1200`,`EDU_2100`,`EDU_2000`) VALUES (11,"Y","Y","N"),(12,"Y","Y","N"),(13,"Y","N","N"),(14,"Y","N","N"),(15,"Y","N","N"),(16,"N","Y","N"),(17,"Y","N","Y"),(18,"N","N","N"),(19,"Y","Y","N"),(20,"N","N","N");

/* Flags Data */
INSERT INTO Flags (Student_ID,Flag_Appt,Flag_Testing) VALUES (1,'IP-Needs appt','They have satisfied summer testing requirements and provide documentation of passing scores in the required tests'),(2,'NULL','Provide documentation that they have registered for the two required tests'),(3,'GPA-no appt','Provide documentation that they have registered for the two required tests'),(4,'IP-Needs appt','Awaiting scores from summer tests'),(5,'NULL','They have satisfied summer testing requirements and provide documentation of passing scores in the required tests'),(6,'NULL','They have satisfied summer testing requirements and provide documentation of passing scores in the required tests'),(7,'IP-Needs appt','They have satisfied summer testing requirements and provide documentation of passing scores in the required tests'),(8,'IP-Needs appt','Have not yet passed all required tests but provide documentation that they have re-registered'),(9,'NULL','They have satisfied summer testing requirements and provide documentation of passing scores in the required tests'),(10,'NULL','They have satisfied summer testing requirements and provide documentation of passing scores in the required tests');
INSERT INTO Flags (Student_ID,Flag_Appt,Flag_Testing) VALUES (11,'NULL','They have satisfied summer testing requirements and provide documentation of passing scores in the required tests'),(12,'IP-Needs appt','Have not yet passed all required tests but provide documentation that they have re-registered'),(13,'NULL','They have satisfied summer testing requirements and provide documentation of passing scores in the required tests'),(14,'NULL','They have satisfied summer testing requirements and provide documentation of passing scores in the required tests'),(15,'NULL','Provide documentation that they have registered for the two required tests'),(16,'NULL','Awaiting scores from summer tests'),(17,'NULL','Provide documentation that they have registered for the two required tests'),(18,'IP-Needs appt','Awaiting scores from summer tests'),(19,'NULL','They have satisfied summer testing requirements and provide documentation of passing scores in the required tests'),(20,'NULL','They have satisfied summer testing requirements and provide documentation of passing scores in the required tests');

/* J_Year_GPA Data */
INSERT INTO J_Year_GPA (Student_ID,Beg_Fall_GPA,End_Fall_GPA,End_Spring_GPA) VALUES (1,3.2787,3.4463,3.5174),(2,3.4646,3.604,3.6287),(3,3.7826,3.7861,3.8221),(4,3.9262,3.9495,3.9564),(5,3.6125,3.7226,3.759),(6,3.7074,3.7663,3.7705),(7,3.0977,3.5278,3.5793),(8,3.5486,3.4346,3.4686),(9,3.7433,3.7663,3.787),(10,3.2369,3.4887,3.5532);
INSERT INTO J_Year_GPA (Student_ID,Beg_Fall_GPA,End_Fall_GPA,End_Spring_GPA) VALUES (11,2.7971,3.088,3.1842),(12,2.9459,3.1119,3.1377),(13,3.1877,3.3129,3.3386),(14,3.0449,2.8912,2.8654),(15,3.6738,3.7545,3.7825),(16,3.1877,3.4238,3.4789),(17,2.9055,3.0812,3.1305),(18,3.5754,3.6812,3.7122),(19,3.0381,3.3355,3.4224),(20,3.6333,2.7834,3.387);

/* Testing_for_License Data */
Insert INTO Testing_for_License (Student_ID,LTest_ID,Score) VALUES (1,2,156),(1,3,230),(2,2,228),(2,3,255),(3,1,175),(4,2,219),(5,3,234),(6,1,145),(7,3,240),(8,2,256),(9,1,150),(9,3,200),(10,3,234);
Insert INTO Testing_for_License (Student_ID,LTest_ID,Score) VALUES (11,3,229),(12,3,229),(13,1,151),(14,3,230),(15,1,154),(16,1,175),(17,1,200),(17,3,240),(18,3,238),(19,2,227),(20,2,230);


