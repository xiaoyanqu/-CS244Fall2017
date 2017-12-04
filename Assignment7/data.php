<?php
$data = json_decode(file_get_contents('php://input'), true);
$fp = fopen('/Library/Webserver/Data/request.log', 'a');

for ($x = 0; $x < 50; $x++) {
	fwrite($fp, (string)$data["Time"][$x]." ");
	fwrite($fp, (string)$data["Red"][$x]." ");
	fwrite($fp, (string)$data["IR"][$x]." ");
	fwrite($fp, (string)$data["X"][$x]." ");
	fwrite($fp, (string)$data["Y"][$x]." ");
	fwrite($fp, (string)$data["Z"][$x]."\n");    
} 

fclose($fp);