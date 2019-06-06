open Js.Promise;
open PromEx;

[@bs.val] external setTimeout : (((unit) => unit), int) => unit = "";
let p = Js.Promise.make((~resolve, ~reject as _) => {
    setTimeout(u => [@bs] resolve(u), 10);
});

resolve(2)
|> map((x) => x * 2)
|> tap((_) => {
    Js.log("ok");
    resolve()
})
|> tap((ok) => Js.log(ok))
|> thenResolve("lol")
|> amend(Js.String.length)
|> flatAmend(((_, n)) => resolve(n * 10))
|> tap(_ => p |> tap(_ => Js.log("delayed tap")))
|> map(Js.log)
|> thenResolve(Some(9))
|> tapMaybe(Js.log2("it's"))
|> tapMaybe((v) => {
    Js.log2("it's still", v);
    resolve();
})
|> thenResolve(None)
|> tapMaybe(Js.log2("not this time"))
|> always(() => Js.log("always1"))
|> then_(_ => {
    Js.log("woah");
    Js.Exn.raiseError("done");
})
|> always(() => Js.log("always2"))
|> catch((exn) => {
    Js.log2("and...", exn);
    resolve ();
});