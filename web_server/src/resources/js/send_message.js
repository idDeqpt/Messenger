async function send_message()
{
	const params = new URLSearchParams(document.location.search);
	let send_body = {
		chat_id: Number(params.get("id")),
		text: document.getElementById("message-text").value
	};
	document.getElementById("message-text").value = "";
	await check_token("http://127.0.0.1:8008/send_message",
					  {method: "POST", body: JSON.stringify(send_body)},
					  async function(response){});
}