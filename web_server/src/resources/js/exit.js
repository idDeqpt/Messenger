function exit()
{
	localStorage.removeItem("access_token");
	localStorage.removeItem("refresh_token");
	window.location.assign('/auth');
}