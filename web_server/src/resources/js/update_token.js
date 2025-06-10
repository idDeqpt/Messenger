async function update_token(url, options, func)
{
	await fetch("http://127.0.0.1:8008/update_token", {
		method: "POST",
		headers: {"Authorization": localStorage.getItem("refresh_token")}
	}).then(response => {
		if (!response.ok)
		{
			console.log("Unauth u");
			window.location.assign("/auth");
		}
		else
		{
			console.log("Auth u");
			response.json().then(data => {
				localStorage.setItem("access_token", data.access_token);
				localStorage.setItem("refresh_token", data.refresh_token);
				if (options.headers == undefined)
					options.headers = {"Authorization": localStorage.getItem("access_token")};
				else
					options.headers["Authorization"] = localStorage.getItem("access_token");
				fetch(url, options).then(response => {func(response)});
			});
		}
	})
}