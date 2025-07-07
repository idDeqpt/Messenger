function change_profile_photo(file)
{
	document.getElementById("main-photo").innerHTML += "<a id=\"main-photo-warning\">Загрузка изображения</a>";
    let reader = new FileReader();
    reader.readAsDataURL(file);
    let send_body;
    reader.onload = () => {
    	const img = new Image();
        img.src = reader.result;
        img.onload = function () {
        	let width = 100;
            const canvas = document.createElement('canvas');
            const ratio = img.height / img.width;
            canvas.width = width;
            canvas.height = width * ratio;

            const ctx = canvas.getContext('2d');
            ctx.drawImage(img, 0, 0, canvas.width, canvas.height);

            const newImageUrl = canvas.toDataURL('image/png');
            send_body = {
				photo_64: newImageUrl.slice(newImageUrl.indexOf(",") + 1)
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
        };
    }
}