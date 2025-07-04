function change_profile_photo(file)
{
	document.getElementById("main-photo").innerHTML += "<a id=\"main-photo-warning\">Загрузка изображения</a>";
    let reader = new FileReader();
    reader.readAsDataURL(file);
    reader.onload = () => {
    	send_body = {
			photo_64: reader.result.slice(reader.result.indexOf(",") + 1)
		};
		check_token("http://" + api_host + "/change_profile_photo", {method: "POST", body: JSON.stringify(send_body)}, function(response){
			console.log(response.status);
			if (response.status == 200)
			{
				document.getElementById("main-photo-current").setAttribute("src", "data:img/png;base64, " + send_body.photo_64);
				document.getElementById("main-photo-warning").remove();
			}
			else
			{
				document.getElementById("main-photo-warning").setAttribute("class", "warning");
				document.getElementById("main-photo-warning").innerHTML = "Ошибка загрузки";
			}
		});
    }
}