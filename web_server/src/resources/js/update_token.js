async function update_token(url, options, func)
{
	const response = await fetch("http://" + api_host + "/update_token", {
		method: "POST",
		headers: {"Authorization": localStorage.getItem("refresh_token")}
	});

	if (response.status == 401)
	{
		console.log("Unauth u");
		window.location.assign("/auth");
	}
	else
	{
		console.log("Auth u");
		const data = await response.json();
		localStorage.setItem("access_token", data.access_token);
		localStorage.setItem("refresh_token", data.refresh_token);

		if (options.headers == undefined)
			options.headers = {"Authorization": localStorage.getItem("access_token")};
		else
			options.headers["Authorization"] = localStorage.getItem("access_token");
		await check_token(url, options, func);
	}
}