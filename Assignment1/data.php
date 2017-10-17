<?php
$req_dump = print_r($_GET, TRUE);
$fp = fopen('/Library/Webserver/Data/request.log', 'a');
fwrite($fp, $req_dump);
fclose($fp);
