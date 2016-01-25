<?php
  // Setting up error reporting
  error_reporting(E_ALL);

  include "config\\mysql_conn.php";
  include "config\\session.php";

  // Setting up the mysql connection
  $conn = new mysqli($db_servername, $db_username, $db_password, $db_name);

  // Connecting to the mysql server
  if ($conn->connect_error)
  {
    // The connection could not be estabilished
    echo "</h1>Mysql connection not estabilished: ".$conn->connect_error."</h1>";
    exit(1);
  }

?>
<html>
  <head>
    <title>Login</title>
  </head>
  <body>
<?php
  switch ($sess_page_id)
  {
    case 1:
      include "pages\\login.php";
      break;
    case 2:
      include "pages\\logout.php";
      break;
    default:
      echo "Unknown page";
      set_login_state(0);
      exit(2);
      break;
  }
?>
  </body>
</html>
