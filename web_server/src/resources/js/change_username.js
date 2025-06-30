function change_username()
{
	let username_value = document.getElementById("main-username-input").value
	check_token("http://" + api_host + "/change_username?username_64=" + btoa(String.fromCodePoint(...(new TextEncoder().encode(username_value)))), {method: "POST"}, function(response) {
		console.log(response.status);
		if (response.status == 409)
		{
			try {document.getElementById("main-warning").remove();} catch{}
			document.getElementById("main-warning-field").innerHTML += "<a id=\"main-warning\" class=\"warning\">Пользователь с таким именем уже существует</a>";
		}
		else
		{
			document.getElementById("main-username-current").innerHTML = username_value;
			try {document.getElementById("main-warning").remove();} catch{}
		}
	});
}