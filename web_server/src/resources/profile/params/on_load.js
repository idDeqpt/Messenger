const params = new URLSearchParams(document.location.search);
let token_payload = localStorage.getItem("access_token").split(".")[1];
let user_id = (params.get("id") == null) ? JSON.parse(atob(token_payload))["id"] : params.get("id");

check_token("http://" + api_host + "/get_profile_data?id=" + user_id, {method: "GET"}, function(response){
	response.json().then(data => {
		console.log(data);
		document.getElementById("main-data-photo-current").setAttribute("src", "data:img/png;base64, " + data.profile_photo_64);
		document.getElementById("main-data-names-username-value-current").innerHTML = data.username;
		document.getElementById("main-data-names-description-current").innerHTML = (data.description == "") ? "No description" : data.description;
		document.getElementById("main-data-photo-warning").remove();
	});
});