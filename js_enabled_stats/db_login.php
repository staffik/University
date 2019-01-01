<?php

// Connection data
$servername = "localhost";
$username = "root";
$password = "pass";
$dbname = "myDB";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    //die("Connection failed: " . $conn->connect_error);
} else {
    //echo "Success connection";
}

?>
