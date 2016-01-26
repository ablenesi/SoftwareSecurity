<html>
<head>
<title>Guestbook</title>
</head>
<body>
<form action="#">
  Name:<br>
  <input type="text" name="name" value="<?php if (isset($_GET['name'])) echo htmlspecialchars($_GET['name']) ?>">
  <br>
  Email:<br>
  <input type="text" name="email" value="<?php if (isset($_GET['email'])) echo htmlspecialchars($_GET['email']) ?>">
  <br>
  Message:<br>
  <input type="text" name="message" value="<?php if (isset($_GET['message'])) echo htmlspecialchars($_GET['message']) ?>">
  <br><br>
  <input type="submit" value="Preview" name="preview">
  <input type="submit" value="Submit" name="submit">
</form>
<br>
<br>
<h1>CLAC</h1>
command: <br>
<form action="#">
  <input type="text" name="calc" value="<?php if (isset($_GET['calc'])) echo $_GET['calc'] ?>">
  <input type="submit" value="Submit" name="submit_calc">
</form>
<?php
if(!empty($_GET['submit_calc'])){
  if(!empty($_GET['calc'])){
    if(preg_match('/[-+]?[0-9]*([-+*\/][-+]?[0-9]+)*$/', htmlspecialchars($_GET['calc']))){
      $a = 0;
      $a = eval('return '.htmlspecialchars($_GET['calc']).';');
      echo "<strong>".$a."</strong>";
    }else{
      echo "<strong>Wrong input ".htmlspecialchars($_GET['calc'])."</strong>";      
    }
  }
}
  
?>
<br>
<br>

<h1>PING</h1>
ip: <br>
<form action="#">
  <input type="text" name="ip" value="<?php if (isset($_GET['ip'])) echo $_GET['ip'] ?>">
  <input type="submit" value="Submit" name="submit_ip">
</form>
<?php
if(!empty($_GET['submit_calc'])){
  if(!empty($_GET['ip'])){
    if(preg_match('/^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/', htmlspecialchars($_GET['ip']))){
      $b = "NO RESULT";
      $b = shell_exec('ping -c 3 '.htmlspecialchars($_GET['ip']));
      echo "<strong>".$b."</strong>";
    }else{
      echo "<strong>"."Wrong input ".htmlspecialchars($_GET['ip'])."</strong>";      
    }
  }
}
  
?>
<br>
<br>
</body>
</html>
<?php
$host="localhost:8889";
$username="root";
$password="root";
$db_name="soft_sec";

$conn = mysqli_connect($host, $username, $password)or die("cannot connect to the server"); 
$conn->select_db($db_name)or die("cannot select database");

if(!empty($_GET['preview']))
{
    # the preview button was clicked
    echo "<div><b>Preview:</b><br/>".htmlspecialchars($_GET['message'])."</div>";
}
else if(!empty($_GET['submit']))
{
    # the submit button was clicked
    $prep = $conn->prepare("INSERT INTO guestbook (name, email, comment, datetime) VALUES (?, ?, ?, now())") or die("Error in prepare");
    $prep->bind_param("sss", htmlspecialchars($_GET['name']), htmlspecialchars($_GET['email']), htmlspecialchars($_GET['message'])) or die("Error in bind_param");
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