USE School;

CREATE TABLE Students(
	id INT PRIMARY KEY NOT NULL,
	name VARCHAR(31) NOT NULL,
	age INT NOT NULL,
	INDEX(name)
)ENGINE=INNODB;

INSERT INTO Students VALUES (1, 'Joe Chen', 31);
INSERT INTO Students VALUES (2, 'Patrick Shih', 28);
INSERT INTO Students VALUES (3, 'Amy Lin', 2);
INSERT INTO Students VALUES (4, 'Kenneth Shaw', 28);

SELECT * FROM Students
WHERE age = 31 OR name = 'Amy Lin';
# Returns Joe Chen & Amy Lin

DELETE FROM Students
WHERE age = 31;
# Go to browse tab, Joe is deleted, add him

SELECT * FROM Students
ORDER BY name;

UPDATE Students SET age = 40
WHELE name = 'Alan Kim'

SELECT sum(age) FROM Students;

SELECT age, count(*) FROM Students
GROUP BY age;

CREATE TABLE Clubs(
	id INT PRIMARY KEY NOT NULL,
	name VARCHAR(31) NOT NULL,
	studentid INT NOT NULL,
	INDEX(name)
)ENGINE=INNODB;

INSERT INTO Clubs VALUES (1, 'Cooking', 1);
INSERT INTO Clubs VALUES (2, 'Kayaking', 2);
INSERT INTO Clubs VALUES (3, 'Drinking', 3);

SELECT s.name FROM Students s
JOIN Clubs c ON s.id = c.studentid;