<?php

$login_form = '<form method="POST">
  UserName:
  <input type="text" name="username" />
  Password
  <input type="password" name="password" />
  <input type="submit" name="submit" />
</form>
';

echo $login_form;

if (isset($_POST["submit"]))
{
  
  echo "Prepared\n";
  if (!($stmt = $conn->prepare("SELECT user_id, rights FROM users where user_name = ? AND password = ?"))) {
     echo "Prepare failed: (" . $mysqli->errno . ") " . $mysqli->error;
  }

  echo "Binded\n";
  if (!$stmt->bind_param("ss", $_POST["username"], $_POST["password"])) {
    echo "Binding parameters failed: (" . $stmt->errno . ") " . $stmt->error;
  }

  echo "Executed\n";
  if (!$stmt->execute()) {
    echo "Execute failed: (" . $stmt->errno . ") " . $stmt->error;
  }

  if (!$stmt->fetch())
  {
    echo "Incorrect username or password<br/>";

    set_login_state(0);
    set_page(3);
  }
  else
  {
    echo $_POST["username"]. " logged in<br/>";
    set_login_state(1);
  }
}

?>
