var doughnutData = [
				{
					value: 40000,
					color:"#F7464A",
					highlight: "#FF5A5E",
					label: "Motors & Motor Drivers"
				},
				{
					value: 25000,
					color: "#46BFBD",
					highlight: "#5AD3D1",
					label: "Registration Fee"
				},
				{
					value: 9000,
					color: "#FDB45C",
					highlight: "#FFC870",
					label: "Sensors & Encoders"
				},
				{
					value: 5000,
					color: "#949FB1",
					highlight: "#A8B3C5",
					label: "Battery & Chargers"
				},
				{
					value: 5000,
					color: "#4D5360",
					highlight: "#616774",
					label: "PCB & Electronic Components"
				},
				{
					value: 14000,
					color: "#4D5360",
					highlight: "#616774",
					label: "Fabrication & Materials"
				},
{
					value: 12000,
					color: "#4D5360",
					highlight: "#616774",
					label: "Pneumatics"
				}


			];

			window.onload = function(){
				var ctx = document.getElementById("chart-area").getContext("2d");
				window.myDoughnut = new Chart(ctx).Doughnut(doughnutData, {responsive : true});


				

			};
