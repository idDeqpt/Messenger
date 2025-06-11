check_token("http://127.0.0.1:8008/get_username", {method: "GET"}, function(response){
	response.json().then(data => {
		document.getElementById("current-username").innerHTML = data.username;
	});
});