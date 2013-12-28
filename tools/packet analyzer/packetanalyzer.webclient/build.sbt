import play.Project._

name := "Packet Analyzer WebClient"

version := "1.0"

libraryDependencies ++= Seq(
  "org.webjars" %% "webjars-play" % "2.2.0", 
  "org.webjars" % "bootstrap" % "3.0.3",
  "org.webjars" % "SlickGrid" % "2.1"    
)

playScalaSettings
