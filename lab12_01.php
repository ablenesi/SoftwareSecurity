<html>
<head>
<title>Guestbook</title>
</head>
<body>
<form action="#">
  Name:<br>
  <input type="text" name="name" value="<?php if (isset($_GET['name'])) echo $_GET['name'] ?>">
  <br>
  Email:<br>
  <input type="text" name="email" value="<?php if (isset($_GET['email'])) echo $_GET['email'] ?>">
  <br>
  Message:<br>
  <input type="text" name="message" value="<?php if (isset($_GET['message'])) echo $_GET['message'] ?>">
  <br><br>
  <input type="submit" value="Preview" name="preview">
  <input type="submit" value="Submit" name="submit">
</form>
</body>
</html>
<?php

$host="localhost";
$username="guestbook";
$password="hr6X2BfvmdUqMjVe";
$db_name="guestbook";

$conn = mysqli_connect($host, $username, $password)or die("cannot connect to the server"); 
$conn->select_db($db_name)or die("cannot select database");

if(!empty($_GET['preview']))
{
    # the preview button was clicked
    echo "<div><b>Preview:</b><br/>".$_GET['message']."</div>";
}
else if(!empty($_GET['submit']))
{
    # the submit button was clicked
    $prep = $conn->prepare("INSERT INTO guestbook (name, email, comment, datetime) VALUES (?, ?, ?, now())") or die("Error in prepare");
    $prep->bind_param("sss", $_GET['name'], $_GET['email'], $_GET['message']) or die("Error in bind_param");
    $prep->execute() or die("Error in execute");
    $prep->close() or die("Error in close");
}
?>
Comments:
<?php
$result=$conn->query("SELECT * FROM guestbook");
while($rows=$result->fetch_array()){
?>

<table width="400" border="0" align="center" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
<tr>
<td><table width="400" border="0" cellpadding="3" cellspacing="1" bgcolor="#FFFFFF">
<tr>
<td width="117">Name</td>
<td width="14">:</td>
<td width="357"><?php echo $rows['name']; ?></td>
</tr>
<tr>
<td>Email</td>
<td>:</td>
<td><?php echo $rows['email']; ?></td>
</tr>
<tr>
<td valign="top">Comment</td>
<td valign="top">:</td>
<td><?php echo $rows['comment']; ?></td>
</tr>
<tr>
<td valign="top">Date/Time </td>
<td valign="top">:</td>
<td><?php echo $rows['datetime']; ?></td>
</tr>
</table></td>
</tr>
</table>

<?php
}
$conn->close(); //close database
?>