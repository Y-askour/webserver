<?php

session_start();

function generate_new_session_id($length) {
    $characters = 'a1b2c3d4e5f6g7h8i9j0klmnopqrstuvwxyz';
    
    $random_string = '';
    for ($i = 0; $i < $length; $i++) {
        $index = rand(0, strlen($characters) - 1);
        $random_string .= $characters[$index];
    }
    return $random_string;
}

if (isset($_SESSION['SESSION_ID'])) {
    echo "<h1>SESSION_ID: " . $_SESSION['SESSION_ID'] . "</h1>";
}

else {
    $_SESSION['SESSION_ID'] = generate_new_session_id(32);
    echo "<h1>SESSION_ID: " . $_SESSION['SESSION_ID'] . "</h1>";
}



// server {
//     location /login {
//         method: POST;
//         index login.php;
//         root /var/www/html/php;
//         fastcgi_pass unix:/var/run/php/php7.2-fpm.sock;
//     }
// }