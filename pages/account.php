<h3>Account details</h3>
<?php
if (isset($_POST["changeemail"]) && isset($_POST["token"]))
{
	check_token_post();
    $prep = $conn->prepare("update users set email=? where user_name=?") or die("Error in prepared statement");
    $prep->bind_param("ss", $_POST["new_email"], $sess_user_name) or die("Error in bind_param");
    $prep->execute() or die("Error in execute");
    
    sess_set_user_info($sess_user_name, $_POST['new_email']);
}

echo "Username: $sess_user_name<br/>";
echo "Email address: $sess_user_email<br/>";

?>
<h4>Change email</h4>
<form method="POST" action="/">
  New email address:
  <input type="text" name="new_email" />
  <input type="submit" name="changeemail" value="Change Email" />
  <input type="hidden" name="token" value=<?php echo get_token()?> />
</form>
<?php
if (isset($_POST["changeemail"]) && isset($_POST["token"]))
{
	check_token_post();
    echo "Email succesfully changed to: ".$_POST['new_email']."<br/>";
}
echo "<a href='/?page_id=2&token=".get_token()."'>logout</a></br>";
?>