<?php

// Starting a session
session_start();

/*
$PG_LOGIN = 1;
$PG_LOGOUT= 2;
$PG_MAIN  = 3;
*/

$PAGE_DICT = array(
    1 => "login",
    2 => "logout",
    3 => "main"
);

if (!isset($sess_logged_in) && !isset($_SESSION["logged_in"]))
{
    $sess_logged_in = 0;
}

if (!isset($sess_page_id) && !isset($_SESSION["page_id"]))
{
    $sess_page_id   = 1;
}

if (!isset($sess_user_name) && !isset($_SESSION["user_name"]))
{
    $sess_user_name = "";
}

if (!isset($sess_user_email) && !isset($_SESSION["user_email"]))
{
    $sess_user_email = "";
}

$sess_page_name = "login";

function config_sessions()
{
  global $sess_logged_in;
  global $sess_page_id;
  global $sess_page_name;
  global $sess_user_name;
  global $sess_user_email;
  global $PAGE_DICT;
  
  if (isset($_SESSION["logged_in"]))
  {
    $sess_logged_in = $_SESSION["logged_in"];
  }
  
  if (isset($_SESSION["page_id"]))
  {
    $sess_page_id = $_SESSION["page_id"];  
  }

  $sess_page_name = $PAGE_DICT[$sess_page_id];
  
  if (isset($_SESSION["user_name"]))
  {
    $sess_user_name = $_SESSION["user_name"];
  }

  if (isset($_GET["page_id"]) && isset($_GET["token"]))
  {
    check_token_get();
    $sess_page_id = $_GET["page_id"];
  }
    
  if (isset($_SESSION["user_email"]))
  {
    $sess_user_email = $_SESSION["user_email"];
  }
}

function set_login_state($value)
{
  $sess_logged_in = $value;
  $_SESSION["logged_in"] = $value;
}

function set_page($value)
{
  $sess_page_id = $value;
  $_SESSION["page_id"] = $value;
}

function sess_set_user_info($name, $email)
{
    global $sess_user_name;
    global $sess_user_email;
    
    $sess_user_name = $name;
    $sess_user_email = $email;
    
    $_SESSION["user_name"] = $name;
    $_SESSION["user_email"] = $email;
}

  config_sessions();

// Generating token
function get_token() {
    global $sess_user_name;
    global $sess_page_name;
    return sha1($sess_user_name.$sess_page_name);
}

// Check for token GET
function check_token_get(){
    if(strcmp(get_token(), $_GET["token"]) !== 0){
        die("Preventing CSRF.");
    }
}
// Check for token POST
function check_token_post(){
    if(strcmp(get_token(), $_POST["token"]) !== 0){
        die("Preventing CSRF.");
    }
}

?>
