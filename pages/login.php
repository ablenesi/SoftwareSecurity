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
  $res = $conn->query("SELECT user_id, rights FROM users where user_name='".$_POST["username"]."' AND password='".$_POST["password"]."'");

  if (0 == $res->num_rows)
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
