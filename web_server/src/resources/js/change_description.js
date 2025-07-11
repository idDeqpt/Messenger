function change_description()
{
	let description_value = document.getElementById("main-data-names-description-input").value
	check_token("http://" + api_host + "/change_description?description_64=" + btoa(String.fromCodePoint(...(new TextEncoder().encode(description_value)))), {method: "POST"}, function(response) {
		console.log(response.status);
		if (response.status == 200)
		{
			document.getElementById("main-data-names-description-current").innerHTML = description_value;
			document.getElementById("main-data-names-description-input").value = "";
		}
	});
}