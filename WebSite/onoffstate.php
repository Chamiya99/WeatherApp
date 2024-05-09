<?php
  include 'test_data.php';
  
  //---------------------------------------- Condition to check that POST value is not empty.
  if (!empty($_POST)) {
    // keep track post values
    $id = $_POST['id'];
    
    $myObj = (object)array();
    
    //........................................ 
    $conn = mysqli_connect($hostname,$username,$password,$database);
    $sql = 'SELECT * FROM sensorstate WHERE id="' . $id . '"';
    foreach ($conn->query($sql) as $row) {
      $myObj->id = $row['ID'];
      $myObj->State = $row['ON/OFF'];
      
      
      $myJSON = json_encode($myObj);
      
      echo $myJSON;
    }
    $conn->close();
    //........................................  
  }
  //---------------------------------------- 
?>