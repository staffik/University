<?php

require 'db_login.php';

$js_enabled = $_POST["js_enabled"];

if($js_enabled == false)
{
   $referer = $_SERVER['HTTP_REFERER'];
   $referer = filter_var($referer, FILTER_SANITIZE_URL);
   
   if($referer)
   {
     $sql = "SELECT id, js_total, js_on FROM js_users_stats WHERE page = '$referer'";
     // should use prepared statements to prevent sql injections
     $result = $conn->query($sql);
     
     if($result->num_rows > 0)
     {
       $row = $result->fetch_assoc();
       
       $id = $row["id"];
       $js_total = $row["js_total"] + 1;
       $js_on = $row["js_on"];
       
       $sql = "UPDATE js_users_stats SET js_total=$js_total, js_on=$js_on WHERE id=$id";
       $conn->query($sql);
     }
     else
     {
       $sql = "INSERT INTO js_users_stats (page, js_total, js_on) VALUES ('$referer', 1, 0)";
       // should use prepared statements to prevent sql injections
       $conn->query($sql);
     }
     
   echo  '<form action="" method="post" id="js_check" style="display:none">
          <input type="text" name="js_enabled" value="OK">
          <input type="text" name="referer" value="' . $referer . '">
          </form>
          <script>
            document.getElementById("js_check").submit();
          </script>';
   }
}
else
{
  $referer = $_POST["referer"];
  $referer = filter_var($referer, FILTER_SANITIZE_URL);
  
  $sql = "SELECT id, js_total, js_on FROM js_users_stats WHERE page = '$referer'";
  // should use prepared statements to prevent sql injections
  $result = $conn->query($sql);

  if($result->num_rows > 0)
  {
    $row = $result->fetch_assoc();
    $id = $row["id"];
    $js_total = $row["js_total"];
    $js_on = $row["js_on"] + 1;
    
    $sql = "UPDATE js_users_stats SET js_total=$js_total, js_on=$js_on WHERE id=$id";
    $conn->query($sql);
  }
}

$conn->close();

?>

