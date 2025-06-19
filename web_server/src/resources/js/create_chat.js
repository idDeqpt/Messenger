async function create_chat(type, name)
{
	let send_body = {
		type: type,
		name: name
	};
	await check_token("http://127.0.0.1:8008/create_chat",
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