<?php
	//Assign Varibles
	$hostname = "localhost";
	$username = "root";
	$password = "";
	$database = "sensor_db";

	//Establish Connection to database
	$conn = mysqli_connect($hostname,$username,$password,$database);

	//Check Connection is ok?
	if(!$conn){
		die("Connection failed:" . mysqli_connect_error());
	}

	if(isset($_POST["id"])){
		$id = $_POST["id"];
	}else{

		//Show that connection is ok!
		echo "Database Connection is ok!<br>";

		if(isset($_POST["temperature"]) && isset($_POST["humidity"]) && isset($_POST["presure"])){

			//Hardcorded data
			$intTemp = $_POST["temperature"];
			$intHumi = $_POST["humidity"];
			$intpres = $_POST["presure"];

			//SQL Query
			$sql = "INSERT INTO dht11 (temperature,humidity) VALUES (".$intTemp.",".$intHumi.")";
			$sql2 = "INSERT INTO bmp280 (presure) VALUES (".$intpres.")";

			//Check whether data sent successfully
			if (mysqli_query($conn , $sql) and mysqli_query($conn , $sql2) ){
				echo "New Recode Created Sucessfully!";
			}
			else{
				echo "Error:". $sql . "<br>" . mysqli_error($conn); 
				echo "Error:". $sql2 . "<br>" . mysqli_error($conn);
			}
		
		}else{
			echo "test faild!";
		}
	}

?>

