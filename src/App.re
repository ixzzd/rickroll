open BsReactNative;

type video = {
  title: string,
  path: Packager.required,
};

let videos = [
  {title: "Never Gonna Give You Up", path: Packager.require("./never.mp4")},
  {title: "Together Forever", path: Packager.require("./together.mp4")},
];

type state = {selectedVideo: option(video)};

type action =
  | SelectVideo(video)
  | ClearVideo;

let component = ReasonReact.reducerComponent("App");

let videoStyle = Style.(style([width(Pct(100.)), height(Pct(100.))]));

let make = _children => {
  ...component,
  initialState: () => {selectedVideo: None},
  reducer: (action, _) =>
    switch (action) {
    | SelectVideo(video) => ReasonReact.Update({selectedVideo: Some(video)})
    | ClearVideo => ReasonReact.Update({selectedVideo: None})
    },
  render: self =>
    <View
      style=Style.(
        style([flex(1.), justifyContent(Center), alignItems(Center)])
      )>
      {
        switch (self.state.selectedVideo) {
        | Some(video) =>
          <ReasonExpo.Video
            style=videoStyle
            shouldPlay=true
            source={`Required(video.path)}
            onPlaybackStatusUpdate=(
              playbackStatus =>
                playbackStatus
                ->ReasonExpo.Video.didJustFinishGet
                ->Belt.Option.mapWithDefault((), didJustFinish =>
                    if (didJustFinish) {
                      self.send(ClearVideo);
                    }
                  )
            )
          />
        | None =>
          videos
          |> List.map(video =>
               <Button
                 onPress=(() => self.send(SelectVideo(video)))
                 title={video.title}
               />
             )
          |> Array.of_list
          |> ReasonReact.array
        }
      }
    </View>,
};

let default = ReasonReact.wrapReasonForJs(~component, _jsProps => make([||]));
