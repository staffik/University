## 2016

index.php is the file containing script and showing statistics
include act.php on every page that should run script
include show.php on page that should show statictics

db_login.php contains data required to log in and logs in
set your username, password and database name

init.php should be run once at the beginning in order to create database and table
if database already exist then only create table
or do nothing if appropriate database and table exists
remember to have same database name in db_login.php
if you decided to have another table name instead of 'js_users_stats',
then replace 'js_users_stats' with this name in act.php and show.php files

act.php check if 'js_enabled' variable is sent by POST method
if not, then increment 'js_total' register for 'referer' in database
and place in page a javascript code that send hidden form with 'js_enabled' variable set to "OK"
if user has javascript enabled, then will be redirected to same page
but now 'js_enabled' variable is set, and 'js_on' register for 'referer' will be incremented
second 'referer' is sent by the form in order to prevent treating current page as referer

SQL queries should be done as prepared statements, cause of SQL injection vulnerability
One may improve script to not include page on current domain in statistics.
User may refresh page and resend form, then 'js_on' register will be double incremented, while 'js_total' only once
So as to prevent such situation one may use a cookie and not to increment 'js_on' register anymore if cookie is set
But user may not support cookies, another solution is to set 'js_enabled' in form as some random generated string,
remember that string in another table 'strings'. When redirected check if 'js_enabled' exists in 'strings' table.
Increment 'js_on' and delete 'js_enabled' from 'strings' only when 'js_enabled' is set and exist in 'strings' table.
