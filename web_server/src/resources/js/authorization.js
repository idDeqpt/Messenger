let has_warning = false;


function create_warning_field(text)
{
	if (has_warning)
	{
		document.getElementById("warning-field").innerHTML = "<a class=\"warning\">" + text + "</a>";
		return;
	}
	const el = document.createElement("div");
	el.id = "warning-field";
	el.innerHTML = "<a class=\"warning\">Необходимо заполнить все поля</a>";
	document.getElementById("main").insertBefore(el, document.getElementById("buttons"));
	has_warning = true;
}


function signup()
{
	let login_value = document.getElementById("login").value;
	let password_value = document.getElementById("password").value;
	if ((login_value == "") || (password_value == ""))
	{
		create_warning_field("Необходимо заполнить все поля");
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
			create_warning_field("Пользователь с таким логином уже существует");
	});
}

function login()
{
	let login_value = document.getElementById("login").value;
	let password_value = document.getElementById("password").value;
	if ((login_value == "") || (password_value == ""))
	{
		create_warning_field("Необходимо заполнить все поля");
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
			create_warning_field("Неправильный логин или пароль");
	});
}