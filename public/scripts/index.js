function epochToJsDate(e) {
    return new Date(1e3 * e);
}
function epochToDateTime(e) {
    var t = new Date(epochToJsDate(e));
    return t.getFullYear() + "/" + ("00" + (t.getMonth() + 1)).slice(-2) + "/" + ("00" + t.getDate()).slice(-2) + " " + ("00" + t.getHours()).slice(-2) + ":" + ("00" + t.getMinutes()).slice(-2) + ":" + ("00" + t.getSeconds()).slice(-2);
}
function plotValues(e, t, n) {
    var l = epochToJsDate(t).getTime(),
        a = Number(n);
    e.series[0].data.length > 40 ? e.series[0].addPoint([l, a], !0, !0, !0) : e.series[0].addPoint([l, a], !0, !1, !0);
}
const loginElement = document.querySelector("#login-form"),
    contentElement = document.querySelector("#content-sign-in"),
    userDetailsElement = document.querySelector("#user-details"),
    authBarElement = document.querySelector("#authentication-bar"),
    deleteButtonElement = document.getElementById("delete-button"),
    deleteModalElement = document.getElementById("delete-modal"),
    deleteDataFormElement = document.querySelector("#delete-data-form"),
    viewDataButtonElement = document.getElementById("view-data-button"),
    hideDataButtonElement = document.getElementById("hide-data-button"),
    tableContainerElement = document.querySelector("#table-container"),
    chartsRangeInputElement = document.getElementById("charts-range"),
    loadDataButtonElement = document.getElementById("load-data"),
    cardsCheckboxElement = document.querySelector("input[name=cards-checkbox]"),
    gaugesCheckboxElement = document.querySelector("input[name=gauges-checkbox]"),
    chartsCheckboxElement = document.querySelector("input[name=charts-checkbox]"),
    cardsReadingsElement = document.querySelector("#cards-div"),
    gaugesReadingsElement = document.querySelector("#gauges-div"),
    chartsDivElement = document.querySelector("#charts-div"),
    tempElement = document.getElementById("temp"),
    humElement = document.getElementById("hum"),
    updateElement = document.getElementById("lastUpdate"),
    setupUI = (e) => {
        if (e) {
            (loginElement.style.display = "none"), (contentElement.style.display = "block"), (authBarElement.style.display = "block"), (userDetailsElement.style.display = "block"), (userDetailsElement.innerHTML = e.email);
            var t,
                n = e.uid;
            console.log(n);
            var l = "UsersData/" + n.toString() + "/readings",
                a = "UsersData/" + n.toString() + "/charts/range",
                o = firebase.database().ref(l),
                s = firebase.database().ref(a),
                d = 0;
            function i() {
                var e = !0;
                o.orderByKey()
                    .limitToLast(100)
                    .on("child_added", function (n) {
                        if (n.exists()) {
                            var l = n.toJSON();
                            console.log(l);
                            var a = l.capacity,
                                o = l.humans,
                                s = l.time,
                                d = "";
                            (d += "<tr>"), (d += "<td>" + epochToDateTime(s) + "</td>"), (d += "<td>" + a + "</td>"), (d += "<td>" + o + "</td>"), (d += "</tr>"), $("#tbody").prepend(d), e && ((t = s), (e = !1), console.log(t));
                        }
                    });
            }
            function r() {
                var e = [],
                    n = [];
                console.log("APEND"),
                    o
                        .orderByKey()
                        .limitToLast(100)
                        .endAt(t)
                        .once("value", function (l) {
                            if (l.exists()) {
                                l.forEach((t) => {
                                    var n = t.toJSON();
                                    e.push(n);
                                }),
                                    (t = e[0].time);
                                var a = !0;
                                (n = e.reverse()).forEach((e) => {
                                    if (a) a = !1;
                                    else {
                                        var t = e.capacity,
                                            n = e.humidity,
                                            l = e.time,
                                            o = "";
                                        (o += "<tr>"), (o += "<td>" + epochToDateTime(l) + "</td>"), (o += "<td>" + t + "</td>"), (o += "<td>" + n + "</td>"), (o += "</tr>"), $("#tbody").append(o);
                                    }
                                });
                            }
                        });
            }
            s.on("value", (e) => {
                (d = Number(e.val())),
                    console.log(d),
                    chartT.destroy(),
                    chartH.destroy(),
                    (chartT = createTemperatureChart()),
                    (chartH = createHumidityChart()),
                    o
                        .orderByKey()
                        .limitToLast(d)
                        .on("child_added", (e) => {
                            var t = e.toJSON(),
                                n = t.capacity,
                                l = t.humans,
                                a = t.time;
                            plotValues(chartT, a, n), plotValues(chartH, a, l);
                        });
            }),
                (chartsRangeInputElement.onchange = () => {
                    s.set(chartsRangeInputElement.value);
                }),
                cardsCheckboxElement.addEventListener("change", (e) => {
                    cardsCheckboxElement.checked ? (cardsReadingsElement.style.display = "block") : (cardsReadingsElement.style.display = "none");
                }),
                chartsCheckboxElement.addEventListener("change", (e) => {
                    chartsCheckboxElement.checked ? (chartsDivElement.style.display = "block") : (chartsDivElement.style.display = "none");
                }),
                o
                    .orderByKey()
                    .limitToLast(1)
                    .on("child_added", (e) => {
                        var t = e.toJSON(),
                            n = t.capacity,
                            l = t.humans,
                            a = t.time;
                        (tempElement.innerHTML = n), (humElement.innerHTML = l), (updateElement.innerHTML = epochToDateTime(a));
                    }),
                deleteButtonElement.addEventListener("click", (e) => {
                    console.log("Remove data"), e.preventDefault, (deleteModalElement.style.display = "block");
                }),
                deleteDataFormElement.addEventListener("submit", (e) => {
                    o.remove();
                }),
                viewDataButtonElement.addEventListener("click", (e) => {
                    (tableContainerElement.style.display = "block"), (viewDataButtonElement.style.display = "none"), (hideDataButtonElement.style.display = "inline-block"), (loadDataButtonElement.style.display = "inline-block"), i();
                }),
                loadDataButtonElement.addEventListener("click", (e) => {
                    r();
                }),
                hideDataButtonElement.addEventListener("click", (e) => {
                    (tableContainerElement.style.display = "none"), (viewDataButtonElement.style.display = "inline-block"), (hideDataButtonElement.style.display = "none");
                });
        } else (loginElement.style.display = "block"), (authBarElement.style.display = "none"), (userDetailsElement.style.display = "none"), (contentElement.style.display = "none");
    };
