<?php

echo "<table border=\"5px solid black\">";
echo "<tr>";
echo "<th>" . "ID" . "</th>"; 
echo "<th>" . "JS-OFF" . "</th>";
echo "<th>" . "JS-ON" . "</th>";
echo "<th>" . "TOTAL" . "</th>";
echo "<th>" . "PAGE" . "</th>";
echo "</tr>"; 

require 'db_login.php';

$sql = "SELECT * FROM js_users_stats";
$result = $conn->query($sql);

while($row = $result->fetch_assoc())
{
  $id = $row["id"];
  $page = $row["page"];
  $totalNumber = $row["js_total"];
  $jsEnabledNumber = $row["js_on"];
  $jsDisabledNumber = $totalNumber - $jsEnabledNumber;
  
  echo "<tr>";
  echo "<td>" . "$id" . "</td>";
  echo "<td>" . "$jsDisabledNumber" . "</td>";
  echo "<td>" . "$jsEnabledNumber" . "</td>";
  echo "<td>" . "$totalNumber" . "</td>";
  echo "<td>" . "$page" . "</td>";
  echo "</tr>";
}

// close connection
$conn->close();

echo "</table>";

?>

