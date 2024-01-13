

<?php
    if (isset($_COOKIE['user'])) {
        $user = $_COOKIE['user'];
        if ($user != null) {
            echo "<h1>hello $user</h1>";
        }
    }
    else
	{
        echo '<html>
		<head>
			<style>
				/* Add some basic styling */
				body {
					font-family: Arial, sans-serif;
					background-color: #f2f2f2;
					padding: 20px;
				}

				.form-container {
					max-width: 300px;
					margin: 0 auto;
					background-color: #fff;
					padding: 20px;
					border-radius: 5px;
					box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
				}

				.form-container h2 {
					text-align: center;
					margin-bottom: 20px;
				}

				.form-container input[type="text"] {
					width: 100%;
					padding: 10px;
					margin-bottom: 10px;
					border-radius: 5px;
					border: 1px solid #ccc;
				}

				.form-container button[type="submit"] {
					width: 100%;
					padding: 10px;
					background-color: #4CAF50;
					color: #fff;
					border: none;
					border-radius: 5px;
					cursor: pointer;
				}

				.form-container button[type="submit"]:hover {
					background-color: #45a049;
				}
			</style>
		</head>
		<body>
			<div class="form-container">
				<h2>Submit User</h2>
				<form method="POST" action="Cookies_set.php">
					<input type="text" name="user" placeholder="Enter username">
					<button type="submit">Submit</button>
				</form>
			</div>
		</body>
	</html>
	';
    }
?>
