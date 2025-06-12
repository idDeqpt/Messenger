async function check_token(url, options, func)
{
	if (options.headers == undefined)
		options.headers = {"Authorization": localStorage.getItem("access_token")};
	else
		options.headers["Authorization"] = localStorage.getItem("access_token");

	const response = await fetch(url, options);
	if (response.status == 401)
	{
		console.log("Unauth c");
		await update_token(url, options, func);
	}
	else
	{
		console.log("Auth c");
		await func(response);
	}
}