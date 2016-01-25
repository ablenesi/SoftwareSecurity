<?php

// Starting a session
session_start();

/*
$PG_LOGIN = 1;
$PG_LOGOUT= 2;
$PG_MAIN  = 3;
*/

$sess_logged_in = 0;
$sess_page_id   = 1;

function config_sessions()
{
  global $sess_logged_in;
  global $sess_page_id;

  if (isset($_SESSION["logged_in"]))
  {
    $sess_logged_in = $_SESSION["logged_in"];
  }

  if (isset($_GET["page_id"]))
  {
    $sess_page_id = $_GET["page_id"];
  }

  echo $_SESSION["logged_in"]." ".$sess_page_id;
}

function set_login_state($value)
{
  $sess_logged_in = $value;
  $_SESSION["logged_in"] = $value;
}

function set_page($value)
{
  $sess_page_id = $value;
  $_GET["page_id"] = $value;
}

  config_sessions();

?>
