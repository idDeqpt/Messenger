async function check_token(url, options, func)
{
	if (options.headers == undefined)
		options.headers = {"Authorization": localStorage.getItem("access_token")};
	else
		options.headers["Authorization"] = localStorage.getItem("access_token");

	await fetch(url, options).then(response => {
		if (!response.ok)
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
check_token("http://127.0.0.1:8008/check_access_token", {
	method: "GET"
	//headers: {"Authorization": localStorage.getItem("access_token")}
}, func => {console.log("success");});