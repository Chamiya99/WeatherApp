//Update Sensor State From Page to Database

<?php
$hostname = "localhost";
$username = "root";
$password = "";
$database = "sensor_db";

$db = new mysqli($hostname, $username, $password, $database);

if ($db->connect_error) {
    die("Connection failed: " . $db->connect_error);
}

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $recordId = $_POST["recordId"];
    $newState = $_POST["newState"];

    $query = "UPDATE sensorstate SET `ON/OFF` = $newState WHERE ID = 1";

    if ($db->query($query) === TRUE) {
        echo "Record updated successfully";
    } else {
        echo "Error updating record: " . $db->error;
    }
}

$db->close();
?>
