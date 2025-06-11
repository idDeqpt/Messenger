async function check_token(url, options, func)
{
	if (options.headers == undefined)
		options.headers = {"Authorization": localStorage.getItem("access_token")};
	else
		options.headers["Authorization"] = localStorage.getItem("access_token");

	await fetch(url, options).then(response => {
		if (response.status == 401)
		{
			console.log("Unauth c");
			update_token(url, options, func);
		}
		else
		{
			console.log("Auth c");
			func(response);
		}
	});
}