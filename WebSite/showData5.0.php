<!DOCTYPE html>
<html>
<head>
    <title>Sensor Data</title>
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://code.jquery.com/jquery-3.5.1.min.js"></script>
    <link rel="stylesheet" href="Style.css"> 
</head>
<body>
    <h1 class="display-4">Weather Station 5.0 - Admin</h1>
    <hr class="hr hr-blurry" />

    <div class="container chart-container">
        <div class="chart">
            <h2 class="mb-4">Temperature Chart</h2>
            <canvas id="temperatureChart" width="400" height="200"></canvas>
        </div>

        <div class="chart">
            <h2 class="mb-4">Humidity Chart</h2>
            <canvas id="humidityChart" width="400" height="200"></canvas>
        </div>

        <div class="chart">
            <h2 class="mb-4">Pressure Chart</h2>
            <canvas id="pressureChart" width="400" height="200"></canvas>
        </div>
        <!-- Sidebar -->
        <div class="container">
            <div class="card">
                <div class="card header">
                    <h3 style="font-size: 1rem;text-align: center;">System Controlling</h3>
                </div>
                <br>
                <label class="switch">
                    <input type="checkbox" id="chkbtn" data-record-id="<?php echo $row['id']; ?>" onclick="updateSystemState(this)">
                    <div class="sliderTS"></div>
                </label>

            </div>  
        </div>
    </div>
    <div class="container">
        <h2 class="mb-4">Weather Station Configuration</h2>
        <form action="updateConfig.php" method="post">
            <div class="form-group">
                <label for="Location">Location:</label>
                <input type="text" class="form-control" name="Location" id="Location" required>
            </div>
            <button type="submit" class="btn btn-primary">Save Configuration</button>
        </form>
    </div>


    <div class="container">
        <h2 class="mb-4">Sensor Data</h2>
        <table class="table table-striped table-bordered">
            <thead class="thead-dark">
                <tr>
                    <th>ID</th>
                    <th>Temperature</th>
                    <th>Humidity</th>
                    <th>Pressure</th>
                    <th>Date/Time</th>
                </tr>
            </thead>
            <tbody>
                <?php

                $hostname = "localhost";
                $username = "root";
                $password = "";
                $database = "sensor_db";
                // Connect to the MySQL database
                $db = new mysqli($hostname,$username,$password,$database);

                // Check for connection errors
                if ($db->connect_error) {
                die("Connection failed: " . $db->connect_error);
                }

                // Fetch data from the DHT11 table and BMP280 table
                $query = "SELECT dht11.*, bmp280.presure FROM dht11 JOIN bmp280 ON dht11.id = bmp280.ID ORDER BY id DESC LIMIT 20";
                $result = $db->query($query);

                if ($result->num_rows > 0) {
                while ($row = $result->fetch_assoc()) {
                    echo "<tr>";
                    echo "<td>" . $row['id'] . "</td>";
                    echo "<td>" . $row['temperature'] . "</td>";
                    echo "<td>" . $row['humidity'] . "</td>";
                    echo "<td>". $row["presure"] . "</td>";
                    echo "<td>" . $row['date/time'] . "</td>";
                    echo "</tr>";
                }
                }

                // Close the database connection
                $db->close();
                ?>
            </tbody>
        </table>
    </div>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.3/dist/umd/popper.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
    <script src="Script.js"></script>
</body>
</html>
