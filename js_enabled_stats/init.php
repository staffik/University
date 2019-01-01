<?php

require 'db_login.php';

// Create database
$sql = "CREATE DATABASE myDB";
if ($conn->query($sql) === TRUE) {
    echo "Database created successfully";
} else {
    echo "Error creating database: " . $conn->error;
}

$sql = "DROP TABLE js_users_stats";

if ($conn->query($sql) === TRUE) {
    echo "Table stats dropped successfully";
} else {
    echo "Error dropping table: " . $conn->error;
}

// sql to create table
$sql = "CREATE TABLE js_users_stats (
id INT(8) UNSIGNED AUTO_INCREMENT PRIMARY KEY, 
page VARCHAR(256) NOT NULL,
js_total INT(8) UNSIGNED DEFAULT 0,
js_on INT(8) UNSIGNED DEFAULT 0
)";

if ($conn->query($sql) === TRUE) {
    echo "Table stats created successfully";
} else {
    echo "Error creating table: " . $conn->error;
}


// close connection
$conn->close();
?>

