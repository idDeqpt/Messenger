function signup()
{
	let login_value = document.getElementById("login").value;
	let password_value = document.getElementById("password").value;
	if ((login_value == "") || (password_value == ""))
	{
		document.getElementById("warning-field").innerHTML = "<a class=\"warning\">Необходимо заполнить все поля</a>";
		return;
	}

	const params = new URLSearchParams({
		login: login_value,
		password: password_value
	}).toString();

	fetch("http://" + api_host + "/signup?" + params, {method: "POST"}).then(response => {
		console.log("fetch", response.ok);
		console.log(response.headers);

		if (response.ok)
		{
			response.json().then(data => {
				window.localStorage.setItem("access_token", data.access_token);
				window.localStorage.setItem("refresh_token", data.refresh_token);
			});
			window.location.assign("/chats");
		}
		else
			document.getElementById("warning-field").innerHTML = "<a class=\"warning\">Пользователь с таким логином уже существует</a>";
	});
}

function login()
{
	let login_value = document.getElementById("login").value;
	let password_value = document.getElementById("password").value;
	if ((login_value == "") || (password_value == ""))
	{
		document.getElementById("warning-field").innerHTML = "<a class=\"warning\">Необходимо заполнить все поля</a>";
		return;
	}

	const params = new URLSearchParams({
		login: login_value,
		password: password_value
	}).toString();
	fetch("http://" + api_host + "/login?" + params, {method: "POST"}).then(response => {
		console.log("fetch", response.ok);
		console.log(response);

		if (response.ok)
		{
			response.json().then(data => {
				window.localStorage.setItem("access_token", data.access_token);
				window.localStorage.setItem("refresh_token", data.refresh_token);
			});
			window.location.assign("/chats");
		}
		else
			document.getElementById("warning-field").innerHTML = "<a class=\"warning\">Неправильный логин или пароль</a>";
	});
}