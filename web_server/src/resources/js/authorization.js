function signup()
{
	let login_value = document.getElementById("login").value;
	let password_value = document.getElementById("password").value;
	const params = new URLSearchParams({
		login: login_value,
		password: password_value
	}).toString();

	fetch("http://127.0.0.1:8008/signup?" + params, {method: "POST"}).then(response => {
		console.log("fetch", response.ok);
		console.log(response.headers);

		if (response.ok)
		{
			window.location.assign("/chats");
			response.json().then(data => {
				localStorage.setItem("access_token", data.access_token);
				localStorage.setItem("refresh_token", data.refresh_token);
			});
		}
		else
			document.getElementById("warning-field").innerHTML = "<a class=\"warning\">Пользователь с таким логином уже существует</a>";
	});
}

function login()
{
	let login_value = document.getElementById("login").value;
	let password_value = document.getElementById("password").value;
	console.log("login: ", login_value);
	console.log("password: ", password_value);

	const params = new URLSearchParams({
		login: login_value,
		password: password_value
	}).toString();
	fetch("http://127.0.0.1:8008/login?" + params, {method: "POST"}).then(response => {
		console.log("fetch", response.ok);
		console.log(response);

		if (response.ok)
		{
			window.location.assign("/chats");
			response.json().then(data => {
				localStorage.setItem("access_token", data.access_token);
				localStorage.setItem("refresh_token", data.refresh_token);
			});
		}
		else
			document.getElementById("warning-field").innerHTML = "<a class=\"warning\">Неправильный логин или пароль</a>";
	});
}