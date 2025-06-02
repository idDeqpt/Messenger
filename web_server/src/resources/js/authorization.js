function auth()
{
	let login_value = document.getElementById("login").value;
	let password_value = document.getElementById("password").value;
	console.log("login: ", login_value);
	console.log("password: ", password_value);

	const params = new URLSearchParams({
		login: login_value,
		password: password_value
	}).toString(); 
	fetch("http://127.0.0.1:8008/authorize?" + params);
}