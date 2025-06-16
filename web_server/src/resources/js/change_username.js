function change_username()
{
	let username_value = document.getElementById("username").value
	check_token("http://127.0.0.1:8008/change_username?username=" + username_value, {method: "POST"}, function(response) {
		console.log(response.status);
		if (response.status == 409)
			document.getElementById("warning-field").innerHTML = "<a class=\"warning\">Пользователь с таким именем уже существует</a>";
		else
		{
			document.getElementById("current-username").innerHTML = username_value;
			document.getElementById("warning-field").innerHTML = "";
		}
	});
}