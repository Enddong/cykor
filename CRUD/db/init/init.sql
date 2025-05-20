use exampledb;

CREATE TABLE users(
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50),
    email VARCHAR(255),
    password VARCHAR(255),
    age INT
);

CREATE TABLE posts(
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT,
    title VARCHAR(255),
    content TEXT,
    writetime DATETIME DEFAULT CURRENT_TIMESTAMP
);