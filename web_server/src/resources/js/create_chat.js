async function create_chat(type, name)
{
	let send_body = {
		type: type,
		name: name
	};
	await check_token("http://" + api_host + "/create_chat",
		{method: "POST", body: JSON.stringify(send_body)},
		async function(response){
			if (response.status == 404)
				document.getElementById("warning-field").innerHTML = "Пользователь не найден";
			else
			{
				let data = await response.json();
				window.location.assign('/chat?id=' + data.chat_id);
			}
	});
}