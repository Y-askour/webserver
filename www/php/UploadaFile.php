<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_FILES['file'])) 
{
    $file = $_FILES['file'];
    
    // File information
    $filename = $file['name'];
    $filedata = $file['tmp_name'];
    
    // CGI script URL
    $cgiUrl = 'http://localhost:8081/upload/'; // Replace with your CGI script URL
    
    // cURL POST request
    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, $cgiUrl);
    curl_setopt($curl, CURLOPT_POST, true);
    curl_setopt($curl, CURLOPT_POSTFIELDS, [
        'file' => new CURLFile($filedata, mime_content_type($filedata), $filename)
    ]);
    $result = curl_exec($curl);
    
    // Check for cURL errors
    if ($result === false) {
        echo 'cURL Error: ' . curl_error($curl);
    } else {
        echo $result;
    }
    
    curl_close($curl);
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>File Upload</title>
</head>
<body>
    <h1>Upload a File</h1>
    <form method="post" enctype="multipart/form-data">
        <input type="file" name="file" required>
        <button type="submit">Upload</button>
    </form>
</body>
</html>
