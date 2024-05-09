<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $hostname = "localhost";
    $username = "root";
    $password = "";
    $database = "sensor_db";
    
    $Location = $_POST["Location"];

    // Connect to the MySQL database
    $db = new mysqli($hostname, $username, $password, $database);

    // Check for connection errors
    if ($db->connect_error) {
        die("Connection failed: " . $db->connect_error);
    }

    // Update the configurations in the 'configurations' table
    $query = "UPDATE configurations SET LOCATION='$Location' WHERE ID=1";
    $result = $db->query($query);

    if ($result) {
        echo "Configuration updated successfully!";
    } else {
        echo "Failed to update configuration: " . $db->error;
    }

    // Close the database connection
    $db->close();
}
?>
