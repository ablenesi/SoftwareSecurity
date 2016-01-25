<?php

$login_form = '<form method="POST" action="/">
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
  $prep = $conn->prepare("SELECT email FROM users where user_name=? AND password=?") or die("Error in prepare");
  $prep->bind_param("ss", $_POST["username"], $_POST["password"]) or die("Error in bind_param");
  $res = $prep->execute() or die("Error in execute");
  $res = $prep->get_result();
  
  if ($res->num_rows <= 0)
  {
    echo "Incorrect username or password<br/>";

    # logs out the user
    set_login_state(0);
    
    # and brings back to the login page
    set_page(1);
  }
  else
  {
    $row = $res->fetch_assoc();
    
    # logs in the user
    set_login_state(1);
    
    # saves the user informations to the session
    sess_set_user_info($_POST["username"], $row["email"]);
    
    # the current page will be the main page (account informations)
    set_page(3);
    
    # refreshes the page
    echo '<script type="text/javascript">
           window.location = "/"
      </script>';
  }
}

?>
