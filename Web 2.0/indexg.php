
<html>
	<head>
		<title>Strattonstudios</title>
<style type ="text/css">	
	
body {
	margin: 0;
    font-family: Arial;
    font-size: 17px;
}
#nav_wrapper {
	width: 100%;
    text-align: left;
}
#nav {
    background-color: #222;
    width: 100%;
    margin: 0 auto 0 auto;
}
#nav ul: hover {
	text-decoration: underline;
}
#nav ul {
    list-style-type: none;
    padding: 0;
    margin: 0;
    position: relative;
    min-width: 200px;
}
#nav ul li {
    display: inline-block;
}
#nav ul li:hover {
    background-color: #333;
}
#nav ul li a, visited {
    color: #CCC;
    display: block;
    padding: 15px;
    text-decoration: none;
}
#nav ul li:hover ul {
    display: block;
    text-decoration: underline;
}
#nav ul ul {
    display: none;
    position: absolute;
    background-color: #333;
    border: 5px solid #222;
    border-top: 0;
    margin-left: -5px;
}
#nav ul ul li {
    display: block;
}
#nav ul ul li a:hover {
    color: #699;
    padding: 10px;
}
.signup{
	float: right;
}
		</style>
<body>
	<div id="nav">
    <div id="nav_wrapper">
        <ul>
            <li><a href="AboutUs.html">About Me</a></li>
            <li> <a href="NewFile.html">Home</a></li>
            <li> <a href="Projectstab.html">Project</a>

                <ul>
                    <li><a href="#">Arduino/Pi</a></li>
                    <li><a href="#">Protools</a></li>
                    <li><a href="#">Product Reviews</a></li>
                </ul>
            </li>
            <li> <a href="contact.html">Follow Me</a>

                <ul>
                    <li><a href="#">Facebook</a>
                    </li>
                    <li><a href="#">Twitter</a>
                    </li>
                	<li><a href="https://www.linkedin.com/in/strattongregory">LinkedIn</a>
                	</li>
                </ul>
            </li>
 		</ul>	
 	</div>
</div>
<!-- Nav wrapper end -->
<!-- Nav end -->
<!-- On/Off button's picture -->
	<?php
	$val_array = array(0,0,0,0,0,0,0,0);
	//this php script generate the first page in function of the file
	for ( $i= 0; $i<8; $i++) {
		//set the pin's mode to output and read them
		system("gpio mode ".$i." out");
		exec ("gpio read ".$i, $val_array[$i], $return );
	}
	//for loop to read the value
	$i =0;
	for ($i = 0; $i < 8; $i++) {
		//if off
		if ($val_array[$i][0] == 0 ) {
			echo ("<img id='button_".$i."' src='data/img/red/red_".$i.".jpg' onclick='change_pin (".$i.");'/>");
		}
		//if on
		if ($val_array[$i][0] == 1 ) {
			echo ("<img id='button_".$i."' src='data/img/green/green_".$i.".jpg' onclick='change_pin (".$i.");'/>");
		}	 
	}
	?>
	 
	<!-- javascript -->
	<script src="script.js"></script>
    </body>
</html>
</body>
<img src="Images/Strattonstudios.jpg"></img>
</html>